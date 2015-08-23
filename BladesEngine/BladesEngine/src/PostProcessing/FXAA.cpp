#include <vector>

#include "Renderer/Technique.h"
#include "Renderer/TechniqueCache.h"
#include "Renderer/TextureManager.h"
#include "Renderer/VertexAssembly.h"
#include "Renderer/DeviceBufferManager.h"
#include "Renderer/FramebufferManager.h"

#include "PostProcessing/FXAA.h"

#include <iostream>

namespace fsi
{
    FXAA::FXAA(int width, int height, GLRenderer* renderer)
        : m_renderer(renderer)
        , m_technique(std::make_shared<Technique>(m_renderer->getTechniqueCache()->getProgram("fxaa")))
    {
        init(width, height);
    }

    FXAA::~FXAA()
    {
    }

    void FXAA::cleanUpFBO()
    {
        m_renderer->getFramebufferManager()->deleteFramebuffer(m_fbo);
        m_renderer->getTextureManager()->deleteTexture(m_tex);
        m_renderer->getFramebufferManager()->deleteRenderbuffer(m_rbo);
    }

    void FXAA::init(int width, int height)
    {
        m_technique->setUniformAttribute("screenWidth", static_cast<GLfloat>(width));
        m_technique->setUniformAttribute("screenHeight", static_cast<GLfloat>(height));

        m_sampler = m_renderer->getTextureManager()->getSamplerPreset(TextureManager::NearestClamp);
    
        initFBO(width, height);
        initVAO();
    }

    void FXAA::initFBO(int width, int height)
    {
        m_tex = m_renderer->getTextureManager()->createTexture(1, width, height, InternalFormat::RGBA8);
        auto texInfo = m_renderer->getTextureManager()->createTextureInfo(m_tex, m_sampler);
        m_technique->setUniformAttribute("renderedTexture", texInfo.unit);

        m_rbo = m_renderer->getFramebufferManager()->createRenderbuffer(width, height, GL_DEPTH_COMPONENT24);

        FramebufferDescriptor descriptor;
        descriptor.colorBuffers.emplace_back(m_tex, GL_COLOR_ATTACHMENT0);
        descriptor.depthBuffer = m_rbo;
        descriptor.stencilBuffer = 0;

        m_fbo = m_renderer->getFramebufferManager()->createFramebuffer(descriptor);
    }

    void FXAA::initVAO()
    {
        auto bufferManager = m_renderer->getDeviceBufferManager();

        std::vector<GLfloat> m_quad;
        m_quad.push_back(-1.f); m_quad.push_back(-1.f);
        m_quad.push_back(+1.f); m_quad.push_back(-1.f);
        m_quad.push_back(+1.f); m_quad.push_back(+1.f);
        m_quad.push_back(-1.f); m_quad.push_back(+1.f); 

        GLuint vbo = bufferManager->allocate(m_quad.size() * sizeof(GLfloat), GL_MAP_WRITE_BIT);
        bufferManager->update(vbo, m_quad);

        std::vector<GLushort> m_quadIbo;
        m_quadIbo.push_back(0); m_quadIbo.push_back(1); m_quadIbo.push_back(2);
        m_quadIbo.push_back(0); m_quadIbo.push_back(2); m_quadIbo.push_back(3);

        GLuint ibo = bufferManager->allocate(m_quadIbo.size() * sizeof(GLushort), GL_MAP_WRITE_BIT);
        bufferManager->update(ibo, m_quadIbo);

        VertexLayout layout;
        layout.indexBuffer = ibo;
        layout.vertexBuffers.emplace_back(0, BufferDescriptor{ vbo, 0, 2 * sizeof(GLfloat) });
        layout.attributes.emplace_back(0, AttributeFormat{ VertexAttrib::Position, 2, 0 });
        m_vao = m_renderer->getVertexAssembly()->createInputState(layout);

        bufferManager->deleteBuffer(vbo);
        bufferManager->deleteBuffer(ibo);
    }

    void FXAA::resize(int width, int height)
    {
        m_technique->setUniformAttribute("screenWidth", static_cast<GLfloat>(width));
        m_technique->setUniformAttribute("screenHeight", static_cast<GLfloat>(height));

        cleanUpFBO();
        initFBO(width, height);
    }

    void FXAA::setAsSurface()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
        glClearNamedFramebufferfv(m_fbo, GL_COLOR, 0, defaultColor);
        glClearNamedFramebufferfv(m_fbo, GL_DEPTH, 0, &defaultDepth);
    }

    void FXAA::render()
    {
        m_technique->use();

        glBindVertexArray(m_vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
    }
}