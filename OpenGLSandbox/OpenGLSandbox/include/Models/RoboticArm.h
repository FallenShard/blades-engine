#pragma once

#include "Models/SceneNode.h"

class CubeMesh;
class VertexBuffer;
class IndexBuffer;

class RoboticArm : public SceneNode
{
public:
    RoboticArm(CubeMesh* mesh, ShaderProgram* program, std::vector<SceneNode*>* validationVec);

    ~RoboticArm();

    void update(float timeDelta);
    virtual void render(const glm::mat4& projection, const glm::mat4& view);

private:
    void moveBase(bool increment);
    void moveUpperArm(bool increment);
    void moveLowerArm(bool increment);
    void moveWristPitch(bool increment);
    void moveWristRoll(bool increment);
    void moveFingerOpen(bool increment);

    void buildHierarchy();

    std::vector<SceneNode*>* m_validationVec;

    CubeMesh* m_mesh;

    SceneNode* m_upperArmHolder;
    SceneNode* m_lowerArmHolder;
    SceneNode* m_wristHolder;
    SceneNode* m_leftFingerHolder;
    SceneNode* m_rightFingerHolder;

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
};