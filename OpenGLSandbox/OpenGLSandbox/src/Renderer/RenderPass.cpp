#include "Renderer/RenderPass.h"

RenderPass::RenderPass()
    : m_quadVertexArray(nullptr)
    , m_frameBuffer(nullptr)
    , m_shaderProgram(nullptr)
    , m_fullTex(nullptr)
{

}

RenderPass::~RenderPass()
{
    delete m_fullTex;
    delete m_quadVertexArray;
    glDeleteRenderbuffers(1, &m_depthRenderBuffer);
    delete m_frameBuffer;
}

void RenderPass::attachProgram(ShaderProgram* program)
{
    m_shaderProgram = program;
}

void RenderPass::init(int width, int height)
{
    m_frameBuffer = new FrameBuffer(width, height);
    m_frameBuffer->bind();

    m_fullTex = new Texture(width, height, "renderedTexture");
    m_fullTex->bind();
    m_fullTex->create();
    m_fullTex->setWrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    m_fullTex->setMinFilter(GL_NEAREST);
    m_fullTex->setMagFilter(GL_NEAREST);
    m_fullTex->locate(m_shaderProgram);
    m_frameBuffer->attachTexture(GL_COLOR_ATTACHMENT0, m_fullTex->getTextureId());
    
    glGenRenderbuffers(1, &m_depthRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_depthRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthRenderBuffer);

    m_frameBuffer->enableAttachments();

    m_quadVertexArray = new VertexArray();
    m_quadVertexArray->bind();

    VertexBuffer* qBuffer = new VertexBuffer(GL_STATIC_DRAW);
    qBuffer->bind();
    qBuffer->push(+1.f, -1.f);
    qBuffer->push(-1.f, -1.f);
    qBuffer->push(-1.f, +1.f);
    qBuffer->push(+1.f, -1.f);
    qBuffer->push(-1.f, +1.f);
    qBuffer->push(+1.f, +1.f);
    qBuffer->uploadData();
    qBuffer->setDataCountPerVertex(2);
    m_quadVertexArray->setVertexCount(qBuffer->getVertexCount());
    m_quadVertexArray->attachAttribute(VertexAttribute("position", 2, 0, 0));
    m_quadVertexArray->enableAttributes(m_shaderProgram->getProgramId());

    VertexArray::release();
    Texture::release();
    FrameBuffer::bindScreen();

    delete qBuffer;
}

void RenderPass::activate()
{
    m_frameBuffer->bind();
    FrameBuffer::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderPass::render()
{
    m_shaderProgram->use();

    m_fullTex->activate();
    m_fullTex->bind();
    m_fullTex->setSampler(m_shaderProgram);
    
    m_quadVertexArray->bind();
    m_quadVertexArray->render();
}