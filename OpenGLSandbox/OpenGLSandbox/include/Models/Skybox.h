#pragma once

#include "OpenGL.h"

namespace fsi
{

class ShaderProgram;

class Skybox
{
public:
    Skybox(ShaderProgram* prog);

    ~Skybox();

    void update(const glm::vec3& cameraPos);
    void render(const glm::mat4& projection, const glm::mat4& view);

private:
    ShaderProgram* m_program;

    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ibo;

    GLuint m_texId;
    GLuint m_texUnif;
    GLuint m_sampler;

    glm::mat4 m_modelMatrix;
};

}