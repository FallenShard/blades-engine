#pragma once

#include "OpenGL.h"
#include "ShaderProgram.h"

class Camera
{
public:

    Camera();
    Camera(float fovDegrees);
    Camera(float zNear, float zFar, float frustumScale);

    void set(std::string name, ShaderProgram* prog);
    const float* getProjectionMatrix() const;
    void adjustViewport(int width, int height, ShaderProgram* prog);
    void adjustViewport(int width, int height);

private:
    void setProjectionMatrix(float zNear, float zFar, float frustumScale);
    
    float m_frustumScale;
    float m_zNear;
    float m_zFar;
    glm::mat4 m_projectionMatrix;
};
