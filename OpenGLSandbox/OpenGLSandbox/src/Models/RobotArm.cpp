#include "Models/RobotArm.h"

namespace
{
    const float StandardAngleIncrement = 3.25f;
    const float SmallAngleIncrement    = 2.f;

    inline float clamp(float value, float minValue, float maxValue)
    {
        if (value < minValue)
            return minValue;

        if (value > maxValue)
            return maxValue;

        return value;
    }
}
RobotArm::RobotArm()
    : m_posBase(glm::vec3(3.0f, -5.0f, -40.0f))
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
    , m_vArray(nullptr)
    , m_program(nullptr)
{
}

RobotArm::RobotArm(VertexArray* vArray, ShaderProgram* program)
    : m_posBase(glm::vec3(3.0f, -5.0f, -40.0f))
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
    , m_vArray(vArray)
    , m_program(program)
{
}

void RobotArm::attachVertexArray(VertexArray* vArray)
{
    m_vArray = vArray;
}

void RobotArm::attachShaderProgram(ShaderProgram* program)
{
    m_program = program;
}

void RobotArm::moveBase(bool increment)
{
    m_angBase += increment ? StandardAngleIncrement : -StandardAngleIncrement;
    m_angBase = fmodf(m_angBase, 360.0f);
}

void RobotArm::moveUpperArm(bool increment)
{
    m_angUpperArm += increment ? StandardAngleIncrement : -StandardAngleIncrement;
    m_angUpperArm = clamp(m_angUpperArm, -90.0f, 0.0f);
}

void RobotArm::moveLowerArm(bool increment)
{
    m_angLowerArm += increment ? StandardAngleIncrement : -StandardAngleIncrement;
    m_angLowerArm = clamp(m_angLowerArm, 0.0f, 146.25f);
}

void RobotArm::moveWristPitch(bool increment)
{
    m_angWristPitch += increment ? StandardAngleIncrement : -StandardAngleIncrement;
    m_angWristPitch = clamp(m_angWristPitch, 0.0f, 90.0f);
}

void RobotArm::moveWristRoll(bool increment)
{
    m_angWristRoll += increment ? StandardAngleIncrement : -StandardAngleIncrement;
    m_angWristRoll = fmodf(m_angWristRoll, 360.0f);
}

void RobotArm::moveFingerOpen(bool increment)
{
    m_angFingerOpen += increment ? SmallAngleIncrement : -SmallAngleIncrement;
    m_angFingerOpen = clamp(m_angFingerOpen, 9.0f, 180.0f);
}

void RobotArm::render()
{
    MatrixStack modelToWorldStack;

    m_program->use();
    m_vArray->bind();

    modelToWorldStack.translate(m_posBase);
    modelToWorldStack.rotateY(m_angBase);

    //Draw left base
    modelToWorldStack.push();
    modelToWorldStack.translate(m_posBaseLeft);
    modelToWorldStack.scale(glm::vec3(1.0f, 1.0f, m_scaleBaseZ));
    m_program->setUniformAttribute("modelToWorldMatrix", 1, GL_FALSE, glm::value_ptr(modelToWorldStack.top()));
    m_vArray->renderIndexed();
    modelToWorldStack.pop();


    //Draw right base
    modelToWorldStack.push();
    modelToWorldStack.translate(m_posBaseRight);
    modelToWorldStack.scale(glm::vec3(1.0f, 1.0f, m_scaleBaseZ));
    m_program->setUniformAttribute("modelToWorldMatrix", 1, GL_FALSE, glm::value_ptr(modelToWorldStack.top()));
    m_vArray->renderIndexed();
    modelToWorldStack.pop();

    //Draw main arm.
    drawUpperArm(modelToWorldStack);
}

void RobotArm::drawUpperArm(MatrixStack& modelToWorldStack)
{
    modelToWorldStack.push();
    modelToWorldStack.rotateX(m_angUpperArm);

    modelToWorldStack.push();
    modelToWorldStack.translate(glm::vec3(0.0f, 0.0f, (m_sizeUpperArm / 2.0f) - 1.0f));
    modelToWorldStack.scale(glm::vec3(1.0f, 1.0f, m_sizeUpperArm / 2.0f));
    m_program->setUniformAttribute("modelToWorldMatrix", 1, GL_FALSE, glm::value_ptr(modelToWorldStack.top()));
    m_vArray->renderIndexed();
    modelToWorldStack.pop();

    drawLowerArm(modelToWorldStack);

    modelToWorldStack.pop();
}

void RobotArm::drawLowerArm(MatrixStack &modelToWorldStack)
{
    modelToWorldStack.push();
    modelToWorldStack.translate(m_posLowerArm);
    modelToWorldStack.rotateX(m_angLowerArm);

    modelToWorldStack.push();
    modelToWorldStack.translate(glm::vec3(0.0f, 0.0f, m_lenLowerArm / 2.0f));
    modelToWorldStack.scale(glm::vec3(m_widthLowerArm / 2.0f, m_widthLowerArm / 2.0f, m_lenLowerArm / 2.0f));
    m_program->setUniformAttribute("modelToWorldMatrix", 1, GL_FALSE, glm::value_ptr(modelToWorldStack.top()));
    m_vArray->renderIndexed();
    modelToWorldStack.pop();

    drawWrist(modelToWorldStack);

    modelToWorldStack.pop();
}

void RobotArm::drawWrist(MatrixStack &modelToWorldStack)
{
    modelToWorldStack.push();
    modelToWorldStack.translate(m_posWrist);
    modelToWorldStack.rotateZ(m_angWristRoll);
    modelToWorldStack.rotateX(m_angWristPitch);

    modelToWorldStack.push();
    modelToWorldStack.scale(glm::vec3(m_widthWrist / 2.0f, m_widthWrist / 2.0f, m_lenWrist / 2.0f));
    m_program->setUniformAttribute("modelToWorldMatrix", 1, GL_FALSE, glm::value_ptr(modelToWorldStack.top()));
    m_vArray->renderIndexed();
    modelToWorldStack.pop();

    drawFingers(modelToWorldStack);

    modelToWorldStack.pop();
}

void RobotArm::drawFingers(MatrixStack &modelToWorldStack)
{
    //Draw left finger
    modelToWorldStack.push();
    modelToWorldStack.translate(m_posLeftFinger);
    modelToWorldStack.rotateY(m_angFingerOpen);

        modelToWorldStack.push();
        modelToWorldStack.translate(glm::vec3(0.0f, 0.0f, m_lenFinger / 2.0f));
        modelToWorldStack.scale(glm::vec3(m_widthFinger / 2.0f, m_widthFinger / 2.0f, m_lenFinger / 2.0f));
        m_program->setUniformAttribute("modelToWorldMatrix", 1, GL_FALSE, glm::value_ptr(modelToWorldStack.top()));
        m_vArray->renderIndexed();
        modelToWorldStack.pop();

        //Draw left lower finger
        modelToWorldStack.push();
        modelToWorldStack.translate(glm::vec3(0.0f, 0.0f, m_lenFinger));
        modelToWorldStack.rotateY(-m_angLowerFinger);

            modelToWorldStack.push();
            modelToWorldStack.translate(glm::vec3(0.0f, 0.0f, m_lenFinger / 2.0f));
            modelToWorldStack.scale(glm::vec3(m_widthFinger / 2.0f, m_widthFinger / 2.0f, m_lenFinger / 2.0f));
            m_program->setUniformAttribute("modelToWorldMatrix", 1, GL_FALSE, glm::value_ptr(modelToWorldStack.top()));
            m_vArray->renderIndexed();
            modelToWorldStack.pop();

        modelToWorldStack.pop();

    modelToWorldStack.pop();

    //Draw right finger
    modelToWorldStack.push();
    modelToWorldStack.translate(m_posRightFinger);
    modelToWorldStack.rotateY(-m_angFingerOpen);

        modelToWorldStack.push();
        modelToWorldStack.translate(glm::vec3(0.0f, 0.0f, m_lenFinger / 2.0f));
        modelToWorldStack.scale(glm::vec3(m_widthFinger / 2.0f, m_widthFinger / 2.0f, m_lenFinger / 2.0f));
        m_program->setUniformAttribute("modelToWorldMatrix", 1, GL_FALSE, glm::value_ptr(modelToWorldStack.top()));
        m_vArray->renderIndexed();
        modelToWorldStack.pop();

        //Draw right lower finger
        modelToWorldStack.push();
        modelToWorldStack.translate(glm::vec3(0.0f, 0.0f, m_lenFinger));
        modelToWorldStack.rotateY(m_angLowerFinger);

            modelToWorldStack.push();
            modelToWorldStack.translate(glm::vec3(0.0f, 0.0f, m_lenFinger / 2.0f));
            modelToWorldStack.scale(glm::vec3(m_widthFinger / 2.0f, m_widthFinger / 2.0f, m_lenFinger / 2.0f));
            m_program->setUniformAttribute("modelToWorldMatrix", 1, GL_FALSE, glm::value_ptr(modelToWorldStack.top()));
            m_vArray->renderIndexed();
            modelToWorldStack.pop();

        modelToWorldStack.pop();

    modelToWorldStack.pop();
}
