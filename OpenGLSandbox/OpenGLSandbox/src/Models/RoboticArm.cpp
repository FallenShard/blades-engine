#include "Models/RoboticArm.h"
#include "Models/Prism.h"
#include "Models/TransformNode.h"
#include "OglWrapper/VertexBuffer.h"
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

RoboticArm::RoboticArm(PrismMesh* mesh, ShaderProgram* program)
    : SceneNode(nullptr, program)
    , m_mesh(mesh)
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

void RoboticArm::render(const glm::mat4& projection, const glm::mat4& view)
{
    m_shaderProgram->use();
    m_shaderProgram->setUniformAttribute("MVP", projection * view * m_absoluteTrans);
    m_shaderProgram->setUniformAttribute("MV", view * m_absoluteTrans);

    renderChildren(projection, view);
}

void RoboticArm::buildHierarchy()
{
    m_vertexArray = m_mesh->getVertexArray();
    m_vertexArray->bind();
    m_vertexArray->enableAttributes(m_shaderProgram->getProgramId());
    VertexArray::release();

    // Base transforms
    translate(m_posBase);
    rotateY(m_angBase);
    
    // Base
    SceneNode* baseLeft = new Prism(m_mesh, m_shaderProgram);
    baseLeft->setScale(glm::vec3(1.f, 1.f, m_scaleBaseZ));
    baseLeft->setPosition(m_posBaseLeft);
    attachChild(baseLeft);

    SceneNode* baseRight = new Prism(m_mesh, m_shaderProgram);
    baseRight->setScale(glm::vec3(1.f, 1.f, m_scaleBaseZ));
    baseRight->setPosition(m_posBaseRight);
    attachChild(baseRight);
    
    // Upper arm parent for rotation
    m_upperArmHolder = new TransformNode();
    m_upperArmHolder->setRotationX(m_angUpperArm);
    attachChild(m_upperArmHolder);
    
    SceneNode* upperArm = new Prism(m_mesh, m_shaderProgram);
    upperArm->setScale(glm::vec3(1.0f, 1.0f, m_sizeUpperArm / 2.0f));
    upperArm->setPosition(glm::vec3(0.0f, 0.0f, (m_sizeUpperArm / 2.0f) - 1.0f));
    m_upperArmHolder->attachChild(upperArm);

    // Lower arm parent for rotation
    m_lowerArmHolder = new TransformNode();
    m_lowerArmHolder->setRotationX(m_angLowerArm);
    m_lowerArmHolder->setPosition(m_posLowerArm);
    m_upperArmHolder->attachChild(m_lowerArmHolder);

    SceneNode* lowerArm = new Prism(m_mesh, m_shaderProgram);
    lowerArm->setScale(glm::vec3(m_widthLowerArm / 2.0f, m_widthLowerArm / 2.0f, m_lenLowerArm / 2.0f));
    lowerArm->setPosition(glm::vec3(0.0f, 0.0f, m_lenLowerArm / 2.0f));
    m_lowerArmHolder->attachChild(lowerArm);

    // Wrist parent for rotation
    m_wristHolder = new TransformNode();
    m_wristHolder->setRotationZ(m_angWristRoll);
    m_wristHolder->setRotationX(m_angWristPitch);
    m_wristHolder->setPosition(m_posWrist);
    m_lowerArmHolder->attachChild(m_wristHolder);

    SceneNode* wrist = new Prism(m_mesh, m_shaderProgram);
    wrist->setScale(glm::vec3(m_widthWrist / 2.0f, m_widthWrist / 2.0f, m_lenWrist / 2.0f));
    m_wristHolder->attachChild(wrist);

    // Left finger
    m_leftFingerHolder = new TransformNode();
    m_leftFingerHolder->setRotationY(m_angFingerOpen);
    m_leftFingerHolder->setPosition(m_posLeftFinger);
    m_wristHolder->attachChild(m_leftFingerHolder);

    SceneNode* leftFingerUpper = new Prism(m_mesh, m_shaderProgram);
    leftFingerUpper->setScale(glm::vec3(m_widthFinger / 2.0f, m_widthFinger / 2.0f, m_lenFinger / 2.0f));
    leftFingerUpper->setPosition(glm::vec3(0.0f, 0.0f, m_lenFinger / 2.0f));
    m_leftFingerHolder->attachChild(leftFingerUpper);

    SceneNode* posLeftFingerLower = new TransformNode();
    posLeftFingerLower->setRotationY(-m_angLowerFinger);
    posLeftFingerLower->setPosition(glm::vec3(0.0f, 0.0f, m_lenFinger));
    m_leftFingerHolder->attachChild(posLeftFingerLower);

    SceneNode* leftFingerLower = new Prism(m_mesh, m_shaderProgram);
    leftFingerLower->setScale(glm::vec3(m_widthFinger / 2.0f, m_widthFinger / 2.0f, m_lenFinger / 2.0f));
    leftFingerLower->setPosition(glm::vec3(0.0f, 0.0f, m_lenFinger / 2.0f));
    posLeftFingerLower->attachChild(leftFingerLower);

    // Right finger
    m_rightFingerHolder = new TransformNode();
    m_rightFingerHolder->setRotationY(-m_angFingerOpen);
    m_rightFingerHolder->setPosition(m_posRightFinger);
    m_wristHolder->attachChild(m_rightFingerHolder);

    SceneNode* rightFingerUpper = new Prism(m_mesh, m_shaderProgram);
    rightFingerUpper->setScale(glm::vec3(m_widthFinger / 2.0f, m_widthFinger / 2.0f, m_lenFinger / 2.0f));
    rightFingerUpper->setPosition(glm::vec3(0.0f, 0.0f, m_lenFinger / 2.0f));
    m_rightFingerHolder->attachChild(rightFingerUpper);

    SceneNode* posRightFingerLower = new TransformNode();
    posRightFingerLower->setRotationY(m_angLowerFinger);
    posRightFingerLower->setPosition(glm::vec3(0.0f, 0.0f, m_lenFinger));
    m_rightFingerHolder->attachChild(posRightFingerLower);

    SceneNode* rightFingerLower = new Prism(m_mesh, m_shaderProgram);
    rightFingerLower->setScale(glm::vec3(m_widthFinger / 2.0f, m_widthFinger / 2.0f, m_lenFinger / 2.0f));
    rightFingerLower->setPosition(glm::vec3(0.0f, 0.0f, m_lenFinger / 2.0f));
    posRightFingerLower->attachChild(rightFingerLower);
    
    applyTransformation(glm::mat4(1.f));
}

void RoboticArm::moveBase(bool increment)
{
    m_angBase += increment ? StandardAngleIncrement : -StandardAngleIncrement;
    m_angBase = fmodf(m_angBase, 360.0f);
    setRotationY(m_angBase);
}

void RoboticArm::moveUpperArm(bool increment)
{
    m_angUpperArm += increment ? StandardAngleIncrement : -StandardAngleIncrement;
    m_angUpperArm = clamp(m_angUpperArm, -90.0f, 0.0f);
    m_upperArmHolder->setRotationX(m_angUpperArm);
}

void RoboticArm::moveLowerArm(bool increment)
{
    m_angLowerArm += increment ? StandardAngleIncrement : -StandardAngleIncrement;
    m_angLowerArm = clamp(m_angLowerArm, 0.0f, 146.25f);
    m_lowerArmHolder->setRotationX(m_angLowerArm);
}

void RoboticArm::moveWristPitch(bool increment)
{
    m_angWristPitch += increment ? StandardAngleIncrement : -StandardAngleIncrement;
    m_angWristPitch = clamp(m_angWristPitch, 0.0f, 90.0f);
    m_wristHolder->setRotationX(m_angWristPitch);
}

void RoboticArm::moveWristRoll(bool increment)
{
    m_angWristRoll += increment ? StandardAngleIncrement : -StandardAngleIncrement;
    m_angWristRoll = fmodf(m_angWristRoll, 360.0f);
    m_wristHolder->setRotationZ(m_angWristRoll);
}

void RoboticArm::moveFingerOpen(bool increment)
{
    m_angFingerOpen += increment ? SmallAngleIncrement : -SmallAngleIncrement;
    m_angFingerOpen = clamp(m_angFingerOpen, 9.0f, 180.0f);
    m_leftFingerHolder->setRotationY(m_angFingerOpen);
    m_rightFingerHolder->setRotationY(-m_angFingerOpen);
}