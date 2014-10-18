#pragma once

#include "Transformations/MatrixStack.h"
#include "Renderer/VertexArray.h"
#include "Renderer/ShaderProgram.h"

class RobotArm
{
public:
    RobotArm();
    RobotArm(VertexArray* vArray, ShaderProgram* program);

    void attachVertexArray(VertexArray* vArray);
    void attachShaderProgram(ShaderProgram* program);

    void render();

    void moveBase(bool increment);
    void moveUpperArm(bool increment);
    void moveLowerArm(bool increment);
    void moveWristPitch(bool increment);
    void moveWristRoll(bool increment);
    void moveFingerOpen(bool increment);

private:
    void drawFingers(MatrixStack& modelToCameraStack);
    void drawWrist(MatrixStack& modelToCameraStack);
    void drawLowerArm(MatrixStack& modelToCameraStack);
    void drawUpperArm(MatrixStack& modelToCameraStack);

    glm::vec3 m_posBase;
    float     m_angBase;

    glm::vec3 m_posBaseLeft;
    glm::vec3 m_posBaseRight;
    float     m_scaleBaseZ;

    float     m_angUpperArm;
    float     m_sizeUpperArm;

    glm::vec3 m_posLowerArm;
    float     m_angLowerArm;
    float     m_lenLowerArm;
    float     m_widthLowerArm;

    glm::vec3 m_posWrist;
    float     m_angWristRoll;
    float     m_angWristPitch;
    float     m_lenWrist;
    float     m_widthWrist;

    glm::vec3 m_posLeftFinger;
    glm::vec3 m_posRightFinger;
    float     m_angFingerOpen;
    float     m_lenFinger;
    float     m_widthFinger;
    float     m_angLowerFinger;

    VertexArray*   m_vArray;
    ShaderProgram* m_program;
};