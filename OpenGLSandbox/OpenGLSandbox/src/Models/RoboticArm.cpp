#include "Models/RoboticArm.h"
#include "Models/Prism.h"
#include "Models/TransformNode.h"
#include "Renderer/VertexBuffer.h"
#include "Utils/VertexLoader.h"

#include "Window/Keyboard.h"

#include "gtc/matrix_transform.hpp"

namespace
{
    const float StandardAngleIncrement = 3.25f;
    const float SmallAngleIncrement = 2.f;

    inline float clamp(float val, float min, float max)
    {
        if (val < min)
            return min;
        if (val > max)
            return max;
        return val;
    }
}

RoboticArm::RoboticArm(ShaderProgram* program)
    : SceneNode(nullptr, program)
    , m_posBase(glm::vec3(-10.0f, 1.0f, -10.0f))
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

void RoboticArm::update(float timeDelta)
{
    if (Keyboard::isKeyPressed(Keyboard::A))
    {
        moveBase(true);
    }

    if (Keyboard::isKeyPressed(Keyboard::D))
    {
        moveBase(false);
    }

    if (Keyboard::isKeyPressed(Keyboard::W))
    {
        moveUpperArm(false);
    }

    if (Keyboard::isKeyPressed(Keyboard::S))
    {
        moveUpperArm(true);
    }

    if (Keyboard::isKeyPressed(Keyboard::R))
    {
        moveLowerArm(false);
    }

    if (Keyboard::isKeyPressed(Keyboard::F))
    {
        moveLowerArm(true);
    }

    if (Keyboard::isKeyPressed(Keyboard::T))
    {
        moveWristPitch(false);
    }

    if (Keyboard::isKeyPressed(Keyboard::G))
    {
        moveWristPitch(true);
    }

    if (Keyboard::isKeyPressed(Keyboard::Z))
    {
        moveWristRoll(true);
    }

    if (Keyboard::isKeyPressed(Keyboard::C))
    {
        moveWristRoll(false);
    }

    if (Keyboard::isKeyPressed(Keyboard::Q))
    {
        moveFingerOpen(true);
    }

    if (Keyboard::isKeyPressed(Keyboard::E))
    {
        moveFingerOpen(false);
    }
}

void RoboticArm::render()
{
    m_shaderProgram->use();
    m_shaderProgram->setUniformAttribute("modelToWorldMatrix", m_absoluteTrans);

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

    // Base transforms
    translate(m_posBase);
    rotateY(m_angBase);
    
    // Base
    SceneNode* baseLeft = new Prism();
    baseLeft->scale(glm::vec3(1.f, 1.f, m_scaleBaseZ));
    baseLeft->translate(m_posBaseLeft);
    attachChild(baseLeft);

    SceneNode* baseRight = new Prism();
    baseRight->scale(glm::vec3(1.f, 1.f, m_scaleBaseZ));
    baseRight->translate(m_posBaseRight);
    attachChild(baseRight);
    
    // Upper arm parent for rotation
    m_upperArmHolder = new TransformNode();
    m_upperArmHolder->rotateX(m_angUpperArm);
    attachChild(m_upperArmHolder);
    
    SceneNode* upperArm = new Prism();
    upperArm->scale(glm::vec3(1.0f, 1.0f, m_sizeUpperArm / 2.0f));
    upperArm->translate(glm::vec3(0.0f, 0.0f, (m_sizeUpperArm / 2.0f) - 1.0f));
    m_upperArmHolder->attachChild(upperArm);

    // Lower arm parent for rotation
    m_lowerArmHolder = new TransformNode();
    m_lowerArmHolder->rotateX(m_angLowerArm);
    m_lowerArmHolder->translate(m_posLowerArm);
    m_upperArmHolder->attachChild(m_lowerArmHolder);

    SceneNode* lowerArm = new Prism();
    lowerArm->scale(glm::vec3(m_widthLowerArm / 2.0f, m_widthLowerArm / 2.0f, m_lenLowerArm / 2.0f));
    lowerArm->translate(glm::vec3(0.0f, 0.0f, m_lenLowerArm / 2.0f));
    m_lowerArmHolder->attachChild(lowerArm);

    // Wrist parent for rotation
    m_wristHolder = new TransformNode();
    m_wristHolder->rotateZ(m_angWristRoll);
    m_wristHolder->rotateX(m_angWristPitch);
    m_wristHolder->translate(m_posWrist);
    m_lowerArmHolder->attachChild(m_wristHolder);

    SceneNode* wrist = new Prism();
    wrist->scale(glm::vec3(m_widthWrist / 2.0f, m_widthWrist / 2.0f, m_lenWrist / 2.0f));
    m_wristHolder->attachChild(wrist);

    m_leftFingerHolder = new TransformNode();
    m_leftFingerHolder->rotateY(m_angFingerOpen);
    m_leftFingerHolder->translate(m_posLeftFinger);
    m_wristHolder->attachChild(m_leftFingerHolder);

    SceneNode* leftFingerUpper = new Prism();
    leftFingerUpper->scale(glm::vec3(m_widthFinger / 2.0f, m_widthFinger / 2.0f, m_lenFinger / 2.0f));
    leftFingerUpper->translate(glm::vec3(0.0f, 0.0f, m_lenFinger / 2.0f));
    m_leftFingerHolder->attachChild(leftFingerUpper);

    SceneNode* posLeftFingerLower = new TransformNode();
    posLeftFingerLower->rotateY(-m_angLowerFinger);
    posLeftFingerLower->translate(glm::vec3(0.0f, 0.0f, m_lenFinger));
    m_leftFingerHolder->attachChild(posLeftFingerLower);

    SceneNode* leftFingerLower = new Prism();
    leftFingerLower->scale(glm::vec3(m_widthFinger / 2.0f, m_widthFinger / 2.0f, m_lenFinger / 2.0f));
    leftFingerLower->translate(glm::vec3(0.0f, 0.0f, m_lenFinger / 2.0f));
    posLeftFingerLower->attachChild(leftFingerLower);

    m_rightFingerHolder = new TransformNode();
    m_rightFingerHolder->rotateY(-m_angFingerOpen);
    m_rightFingerHolder->translate(m_posRightFinger);
    m_wristHolder->attachChild(m_rightFingerHolder);

    SceneNode* rightFingerUpper = new Prism();
    rightFingerUpper->scale(glm::vec3(m_widthFinger / 2.0f, m_widthFinger / 2.0f, m_lenFinger / 2.0f));
    rightFingerUpper->translate(glm::vec3(0.0f, 0.0f, m_lenFinger / 2.0f));
    m_rightFingerHolder->attachChild(rightFingerUpper);

    SceneNode* posRightFingerLower = new TransformNode();
    posRightFingerLower->rotateY(m_angLowerFinger);
    posRightFingerLower->translate(glm::vec3(0.0f, 0.0f, m_lenFinger));
    m_rightFingerHolder->attachChild(posRightFingerLower);

    SceneNode* rightFingerLower = new Prism();
    rightFingerLower->scale(glm::vec3(m_widthFinger / 2.0f, m_widthFinger / 2.0f, m_lenFinger / 2.0f));
    rightFingerLower->translate(glm::vec3(0.0f, 0.0f, m_lenFinger / 2.0f));
    posRightFingerLower->attachChild(rightFingerLower);
    
    applyTransformation(glm::mat4(1.f));
}

void RoboticArm::moveBase(bool increment)
{
    m_angBase += increment ? StandardAngleIncrement : -StandardAngleIncrement;
    m_angBase = fmodf(m_angBase, 360.0f);
    rotateY(m_angBase);
}

void RoboticArm::moveUpperArm(bool increment)
{
    m_angUpperArm += increment ? StandardAngleIncrement : -StandardAngleIncrement;
    m_angUpperArm = clamp(m_angUpperArm, -90.0f, 0.0f);
    m_upperArmHolder->rotateX(m_angUpperArm);
}

void RoboticArm::moveLowerArm(bool increment)
{
    m_angLowerArm += increment ? StandardAngleIncrement : -StandardAngleIncrement;
    m_angLowerArm = clamp(m_angLowerArm, 0.0f, 146.25f);
    m_lowerArmHolder->rotateX(m_angLowerArm);
}

void RoboticArm::moveWristPitch(bool increment)
{
    m_angWristPitch += increment ? StandardAngleIncrement : -StandardAngleIncrement;
    m_angWristPitch = clamp(m_angWristPitch, 0.0f, 90.0f);
    m_wristHolder->rotateX(m_angWristPitch);
}

void RoboticArm::moveWristRoll(bool increment)
{
    m_angWristRoll += increment ? StandardAngleIncrement : -StandardAngleIncrement;
    m_angWristRoll = fmodf(m_angWristRoll, 360.0f);
    m_wristHolder->rotateZ(m_angWristRoll);
}

void RoboticArm::moveFingerOpen(bool increment)
{
    m_angFingerOpen += increment ? SmallAngleIncrement : -SmallAngleIncrement;
    m_angFingerOpen = clamp(m_angFingerOpen, 9.0f, 180.0f);
    m_leftFingerHolder->rotateY(m_angFingerOpen);
    m_rightFingerHolder->rotateY(-m_angFingerOpen);
}