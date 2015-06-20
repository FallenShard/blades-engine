#pragma once

#include "OpenGL.h"

namespace fsi
{

class ShaderManager;

class Terrain
{
public:
    Terrain(ShaderManager* manager);
    ~Terrain();

    void update(const float deltaTime);
    void render(const glm::mat4& projection, const glm::mat4& view);
    void resizeScreenSpace(int width, int height);

    void setWireframe(bool enabled);
    void setTriSize(int size);

private:
    void init();

    ShaderProgram* m_program;

    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ibo;
    GLuint m_tex;
    GLuint m_sampler;

    glm::mat4 m_modelMatrix;

    int m_wireframe;
    float m_triSize;
};

}