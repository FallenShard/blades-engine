#include "Renderer/FrameBuffer.h"

namespace
{
    GLuint boundFBO = 0;
}

FrameBuffer::FrameBuffer(int width, int height)
    : m_size(width, height)
{
    glGenFramebuffers(1, &m_id);
}

FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &m_id);
}

void FrameBuffer::bind()
{
    if (boundFBO != m_id)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_id);
        boundFBO = m_id;
    }
}

void FrameBuffer::bindScreen()
{
    if (boundFBO != 0)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        boundFBO = 0;
    }  
}

void FrameBuffer::attachTexture(GLenum target, GLuint texId, GLint level)
{
    glFramebufferTexture2D(GL_FRAMEBUFFER, target, GL_TEXTURE_2D, texId, level);

    auto foundElement = std::find(m_attachmentPoints.begin(), m_attachmentPoints.end(), target);
    if (foundElement == m_attachmentPoints.end())
        m_attachmentPoints.push_back(target);
}

void FrameBuffer::clear(GLbitfield bitMask)
{
    glClear(bitMask);
}

bool FrameBuffer::checkCompleteness()
{
    return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

void FrameBuffer::enableAttachments()
{
    glDrawBuffers(m_attachmentPoints.size(), m_attachmentPoints.data());
}