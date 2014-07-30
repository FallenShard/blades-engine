#ifndef GLCONTEXT_H
#define GLCONTEXT_H

#include <windows.h>

#include "OpenGL.h"

class GLContext
{
public:
    
    // Creates a new OpenGL context for a given device context
    GLContext(HDC deviceContextHandle);

    // Resources clean-up
    ~GLContext();

private:
    void initialize();

    HDC m_deviceContextHandle;
    HGLRC m_resourceContextHandle;

    unsigned int m_majorVersion;
    unsigned int m_minorVersion;
    unsigned int m_depthBufferBits;
    unsigned int m_stencilBufferBits;
    unsigned int m_antiAliasingLevel;
};


#endif