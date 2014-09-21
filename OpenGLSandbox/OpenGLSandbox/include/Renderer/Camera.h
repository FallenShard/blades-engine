#pragma once

#include "OpenGL.h"
#include "Renderer/ShaderProgram.h"

class Camera
{
public:

    Camera();
    Camera(float fovDegrees);
    Camera(float zNear, float zFar, float frustumScale);

    void set(std::string name, ShaderProgram& program);
    void adjustViewport(int width, int height, ShaderProgram& program);

private:
    void setPerspectiveMatrix(float zNear, float zFar, float frustumScale);
    
    float m_frustumScale;
    float m_zNear;
    float m_zFar;
    glm::mat4 m_perspectiveMatrix;
};
