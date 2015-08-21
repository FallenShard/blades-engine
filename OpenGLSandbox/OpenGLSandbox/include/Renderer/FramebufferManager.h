#pragma once

#include <set>
#include <vector>
#include <map>

#include "OpenGL.h"

namespace fsi
{
    struct FramebufferDescriptor
    {
        std::vector<std::pair<GLuint, GLenum>> colorBuffers;
        GLuint depthBuffer;
        GLuint stencilBuffer;
    };

    class FramebufferManager
    {
    public:
        FramebufferManager();
        ~FramebufferManager();

        GLuint createFramebuffer(const FramebufferDescriptor& descriptor);
        GLuint createRenderbuffer(int width, int height, GLenum format);

        void deleteFramebuffer(GLuint framebuffer);
        void deleteRenderbuffer(GLuint renderbuffer);

    private:
        std::map<GLuint, FramebufferDescriptor> m_framebuffers;
        std::set<GLuint> m_renderbuffers;
    };
}