#pragma once

#include "OpenGL.h"

#include <vector>

class FrameBuffer
{
public:
    FrameBuffer(int width, int height);
    ~FrameBuffer();

    void bind();
    static void bindScreen();

    void attachTexture(GLenum attachment, GLuint texId, GLint level = 0);

    static void clear(GLbitfield bitMask);
    bool checkCompleteness();

    void enableAttachments();

private:
    GLuint m_id;

    glm::ivec2 m_size;

    std::vector<GLenum> m_attachmentPoints;
};