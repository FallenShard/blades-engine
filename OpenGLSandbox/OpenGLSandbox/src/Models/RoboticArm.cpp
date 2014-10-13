#include "Models/RoboticArm.h"
#include "Models/Prism.h"
#include "Models/TransformNode.h"
#include "Renderer/VertexBuffer.h"
#include "Utils/VertexLoader.h"

#include "gtc/matrix_transform.hpp"

RoboticArm::RoboticArm(ShaderProgram* program)
    : SceneNode(nullptr, program)
    , m_posBase(glm::vec3(-30.0f, 5.0f, -40.0f))
    , m_angBase(-45.0f)
    , m_posBaseLeft(glm::vec3(2.0f, 0.0f, 0.0f))
    , m_posBaseRight(glm::vec3(-2.0f, 0.0f, 0.0f))
    , m_scaleBaseZ(3.0f)
    , m_angUpperArm(-33.75f)
    , m_sizeUpperArm(9.0f)
    , m_posLowerArm(glm::vec3(0.0f, 0.0f, 8.0f))
    , m_angLowerArm(146.25f)
    , m_lenLowerArm(5.0f)
    , m_widthLowerArm(1.5f)
    , m_posWrist(glm::vec3(0.0f, 0.0f, 5.0f))
    , m_angWristRoll(0.0f)
    , m_angWristPitch(67.5f)
    , m_lenWrist(2.0f)
    , m_widthWrist(2.0f)
    , m_posLeftFinger(glm::vec3(1.0f, 0.0f, 1.0f))
    , m_posRightFinger(glm::vec3(-1.0f, 0.0f, 1.0f))
    , m_angFingerOpen(180.0f)
    , m_lenFinger(2.0f)
    , m_widthFinger(0.5f)
    , m_angLowerFinger(45.0f)
{
    buildHierarchy();
}

RoboticArm::~RoboticArm()
{
    delete m_vertexBuffer;
    delete m_vertexArray;
}

void RoboticArm::render()
{
    m_shaderProgram->use();
    m_shaderProgram->setUniformAttribute("modelToWorldMatrix", 1, GL_FALSE, glm::value_ptr(m_modelMatrix));

    renderChildren();
}

void RoboticArm::buildHierarchy()
{
    m_vertexArray = new VertexArray(GL_TRIANGLES);
    m_vertexArray->bind();

    m_vertexBuffer = new VertexBuffer(GL_STATIC_DRAW);
    m_vertexBuffer->bind();
    VertexLoader vLoader;
    vLoader.loadFromFile("res/RobotArmMesh.txt", *m_vertexBuffer);
    m_vertexArray->setVertexCount(m_vertexBuffer->getVertexCount());

    m_indexBuffer = new IndexBuffer();
    m_indexBuffer->bind();
    m_indexBuffer->loadFromFile("res/RobotArmFaces.txt");
    m_vertexArray->attachIndexBuffer(m_indexBuffer);

    m_vertexArray->attachAttribute(VertexAttribute("vPosition", 3, 0, 0));
    m_vertexArray->attachAttribute(VertexAttribute("vColor", 4, 0, 3 * sizeof(GLfloat) * m_vertexBuffer->getVertexCount()));
    m_vertexArray->enableAttributes(m_shaderProgram->getProgramId());

    VertexArray::release();

    translate(m_posBase);
    rotateY(m_angBase);
    
    SceneNode* baseLeft = new Prism();
    baseLeft->translate(m_posBaseLeft);
    baseLeft->scale(glm::vec3(1.f, 1.f, m_scaleBaseZ));
    attachChild(baseLeft);

    SceneNode* baseRight = new Prism();
    baseRight->translate(m_posBaseRight);
    baseRight->scale(glm::vec3(1.f, 1.f, m_scaleBaseZ));
    attachChild(baseRight);

    SceneNode* posUpperArm = new TransformNode();
    posUpperArm->rotateX(m_angUpperArm);
    attachChild(posUpperArm);

    SceneNode* upperArm = new Prism();
    upperArm->translate(glm::vec3(0.0f, 0.0f, (m_sizeUpperArm / 2.0f) - 1.0f));
    upperArm->scale(glm::vec3(1.0f, 1.0f, m_sizeUpperArm / 2.0f));
    posUpperArm->attachChild(upperArm);

    SceneNode* posLowerArm = new TransformNode();
    posLowerArm->translate(m_posLowerArm);
    posLowerArm->rotateX(m_angLowerArm);
    posUpperArm->attachChild(posLowerArm);

    SceneNode* lowerArm = new Prism();
    lowerArm->translate(glm::vec3(0.0f, 0.0f, m_lenLowerArm / 2.0f));
    lowerArm->scale(glm::vec3(m_widthLowerArm / 2.0f, m_widthLowerArm / 2.0f, m_lenLowerArm / 2.0f));
    posLowerArm->attachChild(lowerArm);

    SceneNode* posWrist = new TransformNode();
    posWrist->translate(m_posWrist);
    posWrist->rotateZ(m_angWristRoll);
    posWrist->rotateX(m_angWristPitch);
    posLowerArm->attachChild(posWrist);

    SceneNode* wrist = new Prism();
    wrist->scale(glm::vec3(m_widthWrist / 2.0f, m_widthWrist / 2.0f, m_lenWrist / 2.0f));
    posWrist->attachChild(wrist);

    SceneNode* posLeftFinger = new TransformNode();
    posLeftFinger->translate(m_posLeftFinger);
    posLeftFinger->rotateY(m_angFingerOpen);
    posWrist->attachChild(posLeftFinger);

    SceneNode* leftFingerUpper = new Prism();
    leftFingerUpper->translate(glm::vec3(0.0f, 0.0f, m_lenFinger / 2.0f));
    leftFingerUpper->scale(glm::vec3(m_widthFinger / 2.0f, m_widthFinger / 2.0f, m_lenFinger / 2.0f));
    posLeftFinger->attachChild(leftFingerUpper);

    SceneNode* posLeftFingerLower = new TransformNode();
    posLeftFingerLower->translate(glm::vec3(0.0f, 0.0f, m_lenFinger));
    posLeftFingerLower->rotateY(-m_angLowerFinger);
    posLeftFinger->attachChild(posLeftFingerLower);

    SceneNode* leftFingerLower = new Prism();
    leftFingerLower->translate(glm::vec3(0.0f, 0.0f, m_lenFinger / 2.0f));
    leftFingerLower->scale(glm::vec3(m_widthFinger / 2.0f, m_widthFinger / 2.0f, m_lenFinger / 2.0f));
    posLeftFingerLower->attachChild(leftFingerLower);

    SceneNode* posRightFinger = new TransformNode();
    posRightFinger->translate(m_posRightFinger);
    posRightFinger->rotateY(-m_angFingerOpen);
    posWrist->attachChild(posRightFinger);

    SceneNode* rightFingerUpper = new Prism();
    rightFingerUpper->translate(glm::vec3(0.0f, 0.0f, m_lenFinger / 2.0f));
    rightFingerUpper->scale(glm::vec3(m_widthFinger / 2.0f, m_widthFinger / 2.0f, m_lenFinger / 2.0f));
    posRightFinger->attachChild(rightFingerUpper);

    SceneNode* posRightFingerLower = new TransformNode();
    posRightFingerLower->translate(glm::vec3(0.0f, 0.0f, m_lenFinger));
    posRightFingerLower->rotateY(m_angLowerFinger);
    posRightFinger->attachChild(posRightFingerLower);

    SceneNode* rightFingerLower = new Prism();
    rightFingerLower->translate(glm::vec3(0.0f, 0.0f, m_lenFinger / 2.0f));
    rightFingerLower->scale(glm::vec3(m_widthFinger / 2.0f, m_widthFinger / 2.0f, m_lenFinger / 2.0f));
    posRightFingerLower->attachChild(rightFingerLower);

    applyTransformation(glm::mat4(1.f));
}