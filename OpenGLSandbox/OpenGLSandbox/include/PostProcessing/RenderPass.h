#pragma once

#include "OglWrapper/ShaderProgram.h"

namespace fsi
{

class RenderPass
{
public:
    RenderPass(int width, int height, ShaderProgram* program);
    ~RenderPass();

    void activate();
    void render();

    void resize(int width, int height);

private:
    void init(int width, int height);
    void initFBO(int width, int height);
    void initVAO();

    void cleanUpFBO();

    GLuint m_vao;

    GLuint m_fbo;
    GLuint m_tex;
    GLuint m_sampler;
    GLuint m_rbo;

    ShaderProgram* m_shaderProgram;
};

}