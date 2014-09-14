#include "Models/RobotArm.h"

namespace
{
    const float StandardAngleIncrement = 3.25f;
    const float SmallAngleIncrement    = 2.f;

    inline float clamp(float fValue, float fMinValue, float fMaxValue)
    {
        if (fValue < fMinValue)
            return fMinValue;

        if (fValue > fMaxValue)
            return fMaxValue;

        return fValue;
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
{
}

void RobotArm::draw(VertexArray& vArray, ShaderProgram& program)
{
    MatrixStack modelToCameraStack;

    program.use();
    vArray.bind();

    modelToCameraStack.translate(m_posBase);
    modelToCameraStack.rotateY(m_angBase);

    //Draw left base
    modelToCameraStack.push();
    modelToCameraStack.translate(m_posBaseLeft);
    modelToCameraStack.scale(glm::vec3(1.0f, 1.0f, m_scaleBaseZ));
    program.setUniformAttribute("modelToCameraMatrix", 1, GL_FALSE, glm::value_ptr(modelToCameraStack.top()));
    vArray.renderIndexed();
    modelToCameraStack.pop();
    

    //Draw right base
    modelToCameraStack.push();
    modelToCameraStack.translate(m_posBaseRight);
    modelToCameraStack.scale(glm::vec3(1.0f, 1.0f, m_scaleBaseZ));
    program.setUniformAttribute("modelToCameraMatrix", 1, GL_FALSE, glm::value_ptr(modelToCameraStack.top()));
    vArray.renderIndexed();
    modelToCameraStack.pop();

    //Draw main arm.
    drawUpperArm(modelToCameraStack, vArray, program);
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

void RobotArm::drawUpperArm(MatrixStack& modelToCameraStack, VertexArray& vArray, ShaderProgram& program)
{
    modelToCameraStack.push();
    modelToCameraStack.rotateX(m_angUpperArm);

    modelToCameraStack.push();
    modelToCameraStack.translate(glm::vec3(0.0f, 0.0f, (m_sizeUpperArm / 2.0f) - 1.0f));
    modelToCameraStack.scale(glm::vec3(1.0f, 1.0f, m_sizeUpperArm / 2.0f));
    program.setUniformAttribute("modelToCameraMatrix", 1, GL_FALSE, glm::value_ptr(modelToCameraStack.top()));
    vArray.renderIndexed();
    modelToCameraStack.pop();

    drawLowerArm(modelToCameraStack, vArray, program);

    modelToCameraStack.pop();
}

void RobotArm::drawLowerArm(MatrixStack &modelToCameraStack, VertexArray& vArray, ShaderProgram& program)
{
    modelToCameraStack.push();
    modelToCameraStack.translate(m_posLowerArm);
    modelToCameraStack.rotateX(m_angLowerArm);

    modelToCameraStack.push();
    modelToCameraStack.translate(glm::vec3(0.0f, 0.0f, m_lenLowerArm / 2.0f));
    modelToCameraStack.scale(glm::vec3(m_widthLowerArm / 2.0f, m_widthLowerArm / 2.0f, m_lenLowerArm / 2.0f));
    program.setUniformAttribute("modelToCameraMatrix", 1, GL_FALSE, glm::value_ptr(modelToCameraStack.top()));
    vArray.renderIndexed();
    modelToCameraStack.pop();

    drawWrist(modelToCameraStack, vArray, program);

    modelToCameraStack.pop();
}

void RobotArm::drawWrist(MatrixStack &modelToCameraStack, VertexArray& vArray, ShaderProgram& program)
{
    modelToCameraStack.push();
    modelToCameraStack.translate(m_posWrist);
    modelToCameraStack.rotateZ(m_angWristRoll);
    modelToCameraStack.rotateX(m_angWristPitch);

    modelToCameraStack.push();
    modelToCameraStack.scale(glm::vec3(m_widthWrist / 2.0f, m_widthWrist / 2.0f, m_lenWrist / 2.0f));
    program.setUniformAttribute("modelToCameraMatrix", 1, GL_FALSE, glm::value_ptr(modelToCameraStack.top()));
    vArray.renderIndexed();
    modelToCameraStack.pop();

    drawFingers(modelToCameraStack, vArray, program);

    modelToCameraStack.pop();
}

void RobotArm::drawFingers(MatrixStack &modelToCameraStack, VertexArray& vArray, ShaderProgram& program)
{
    //Draw left finger
    modelToCameraStack.push();
    modelToCameraStack.translate(m_posLeftFinger);
    modelToCameraStack.rotateY(m_angFingerOpen);

        modelToCameraStack.push();
        modelToCameraStack.translate(glm::vec3(0.0f, 0.0f, m_lenFinger / 2.0f));
        modelToCameraStack.scale(glm::vec3(m_widthFinger / 2.0f, m_widthFinger / 2.0f, m_lenFinger / 2.0f));
        program.setUniformAttribute("modelToCameraMatrix", 1, GL_FALSE, glm::value_ptr(modelToCameraStack.top()));
        vArray.renderIndexed();
        modelToCameraStack.pop();

        //Draw left lower finger
        modelToCameraStack.push();
        modelToCameraStack.translate(glm::vec3(0.0f, 0.0f, m_lenFinger));
        modelToCameraStack.rotateY(-m_angLowerFinger);

            modelToCameraStack.push();
            modelToCameraStack.translate(glm::vec3(0.0f, 0.0f, m_lenFinger / 2.0f));
            modelToCameraStack.scale(glm::vec3(m_widthFinger / 2.0f, m_widthFinger / 2.0f, m_lenFinger / 2.0f));
            program.setUniformAttribute("modelToCameraMatrix", 1, GL_FALSE, glm::value_ptr(modelToCameraStack.top()));
            vArray.renderIndexed();
            modelToCameraStack.pop();

        modelToCameraStack.pop();
    

    modelToCameraStack.pop();

    //Draw right finger
    modelToCameraStack.push();
    modelToCameraStack.translate(m_posRightFinger);
    modelToCameraStack.rotateY(-m_angFingerOpen);

        modelToCameraStack.push();
        modelToCameraStack.translate(glm::vec3(0.0f, 0.0f, m_lenFinger / 2.0f));
        modelToCameraStack.scale(glm::vec3(m_widthFinger / 2.0f, m_widthFinger / 2.0f, m_lenFinger / 2.0f));
        program.setUniformAttribute("modelToCameraMatrix", 1, GL_FALSE, glm::value_ptr(modelToCameraStack.top()));
        vArray.renderIndexed();
        modelToCameraStack.pop();
    
    //Draw right lower finger
        modelToCameraStack.push();
        modelToCameraStack.translate(glm::vec3(0.0f, 0.0f, m_lenFinger));
        modelToCameraStack.rotateY(m_angLowerFinger);

            modelToCameraStack.push();
            modelToCameraStack.translate(glm::vec3(0.0f, 0.0f, m_lenFinger / 2.0f));
            modelToCameraStack.scale(glm::vec3(m_widthFinger / 2.0f, m_widthFinger / 2.0f, m_lenFinger / 2.0f));
            program.setUniformAttribute("modelToCameraMatrix", 1, GL_FALSE, glm::value_ptr(modelToCameraStack.top()));
            vArray.renderIndexed();
            modelToCameraStack.pop();

        modelToCameraStack.pop();

    modelToCameraStack.pop();
}





