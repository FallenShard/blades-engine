#pragma once

#include "OpenGL.h"

namespace fsi
{

class ShaderProgram;

class Panel
{
public:
    Panel(glm::vec3&& position, glm::vec2&& size);
    ~Panel();

    void setPosition(glm::vec3&& position);
    void setSize(glm::vec2&& size);
    void setColor(glm::vec4&& color);

    glm::vec3 getPosition() const;

    void render(ShaderProgram* prog, const glm::mat4& parent);

private:
    glm::vec2 m_size;
    glm::vec3 m_position;
    glm::vec4 m_color;
    glm::vec4 m_borderColor;

    glm::mat4 m_modelMat;

    GLuint m_borderVao;
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ibo;
    GLuint m_borderIbo;

};

}