#include "PostProcessing/RenderPass.h"

namespace fsi
{

RenderPass::RenderPass()
    //: m_quadVertexArray(nullptr)
    //, m_frameBuffer(nullptr)
    //, m_shaderProgram(nullptr)
    //, m_fullTex(nullptr)
{

}

RenderPass::~RenderPass()
{
    glDeleteVertexArrays(1, &m_vao);
    cleanUpFBO();
}

void RenderPass::attachProgram(ShaderProgram* program)
{
    m_shaderProgram = program;
}

void RenderPass::cleanUpFBO()
{
    glDeleteFramebuffers(1, &m_fbo);
    glDeleteTextures(1, &m_tex);
    glDeleteSamplers(1, &m_sampler);
    glDeleteRenderbuffers(1, &m_rbo);
}

void RenderPass::init(int width, int height)
{
    m_shaderProgram->use();
    m_shaderProgram->setUniformAttribute("screenWidth", static_cast<GLfloat>(width));
    m_shaderProgram->setUniformAttribute("screenHeight", static_cast<GLfloat>(height));
    
    initFBO(width, height);
    initVAO();
    
}

void RenderPass::initFBO(int width, int height)
{
    glCreateFramebuffers(1, &m_fbo);

    glCreateTextures(GL_TEXTURE_2D, 1, &m_tex);
    glTextureStorage2D(m_tex, 1, GL_RGBA8, width, height);
    glGenerateTextureMipmap(m_tex);
    glNamedFramebufferTexture(m_fbo, GL_COLOR_ATTACHMENT0, m_tex, 0);

    glCreateRenderbuffers(1, &m_rbo);
    glNamedRenderbufferStorage(m_rbo, GL_DEPTH_COMPONENT24, width, height);
    glNamedFramebufferRenderbuffer(m_fbo, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

    GLenum val = GL_COLOR_ATTACHMENT0;
    glNamedFramebufferDrawBuffers(m_fbo, 1, &val);

    glCreateSamplers(1, &m_sampler);
    glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(m_sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glSamplerParameteri(m_sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTextureUnit(1, m_tex);
    glUniform1i(m_shaderProgram->getUniformAttribute("renderedTexture"), 1);
}

void RenderPass::initVAO()
{
    glCreateVertexArrays(1, &m_vao);

    GLuint vbo;
    glCreateBuffers(1, &vbo);

    std::vector<GLfloat> m_quad;
    m_quad.push_back(+1.f); m_quad.push_back(-1.f);
    m_quad.push_back(-1.f); m_quad.push_back(-1.f);
    m_quad.push_back(-1.f); m_quad.push_back(+1.f);
    m_quad.push_back(+1.f); m_quad.push_back(-1.f);
    m_quad.push_back(-1.f); m_quad.push_back(+1.f);
    m_quad.push_back(+1.f); m_quad.push_back(+1.f);

    glNamedBufferData(vbo, m_quad.size() * sizeof(GLfloat), m_quad.data(), GL_STATIC_DRAW);

    glVertexArrayVertexBuffer(m_vao, VertexBufferBinding::Position, vbo, 0, 2 * sizeof(GLfloat));

    glVertexArrayAttribBinding(m_vao, VertexAttrib::Position, VertexBufferBinding::Position);
    glVertexArrayAttribFormat(m_vao, VertexAttrib::Position, 2, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(m_vao, VertexAttrib::Position);

    glDeleteBuffers(1, &vbo);
}

void RenderPass::resize(int width, int height)
{
    m_shaderProgram->use();
    m_shaderProgram->setUniformAttribute("screenWidth", static_cast<GLfloat>(width));
    m_shaderProgram->setUniformAttribute("screenHeight", static_cast<GLfloat>(height));

    cleanUpFBO();
    initFBO(width, height);
}

void RenderPass::activate()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    FrameBuffer::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderPass::render()
{
    m_shaderProgram->use();

    glActiveTexture(GL_TEXTURE0 + 1);
    glBindVertexArray(m_vao);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

}