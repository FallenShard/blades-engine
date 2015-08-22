#include "Renderer/FramebufferManager.h"

namespace fsi
{
    FramebufferManager::FramebufferManager()
    {

    }

    FramebufferManager::~FramebufferManager()
    {
        for (auto& framebuffer : m_framebuffers)
            glDeleteFramebuffers(1, &framebuffer.first);
        m_framebuffers.clear();

        for (auto& renderbuffer : m_renderbuffers)
            glDeleteRenderbuffers(1, &renderbuffer);
        m_renderbuffers.clear();
    }

    GLuint FramebufferManager::createFramebuffer(const FramebufferDescriptor& descriptor)
    {
        GLuint framebuffer;
        glCreateFramebuffers(1, &framebuffer);

        for (auto& colorBuffer : descriptor.colorBuffers)
        {
            glNamedFramebufferTexture(framebuffer, colorBuffer.second, colorBuffer.first, 0);
            glNamedFramebufferDrawBuffer(framebuffer, colorBuffer.second);
        }
        
        if (descriptor.depthBuffer)
            glNamedFramebufferRenderbuffer(framebuffer, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, descriptor.depthBuffer);

        if (descriptor.stencilBuffer)
            glNamedFramebufferRenderbuffer(framebuffer, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, descriptor.stencilBuffer);

        return framebuffer;
    }

    GLuint FramebufferManager::createRenderbuffer(int width, int height, GLenum format)
    {
        GLuint renderbuffer;
        glCreateRenderbuffers(1, &renderbuffer);
        
        glNamedRenderbufferStorage(renderbuffer, format, width, height);

        m_renderbuffers.emplace(renderbuffer);

        return renderbuffer;
    }

    void FramebufferManager::deleteFramebuffer(GLuint framebuffer)
    {
        glDeleteFramebuffers(1, &framebuffer);
        m_framebuffers.erase(framebuffer);
    }

    void FramebufferManager::deleteRenderbuffer(GLuint renderbuffer)
    {
        glDeleteRenderbuffers(1, &renderbuffer);
        m_renderbuffers.erase(renderbuffer);
    }
}