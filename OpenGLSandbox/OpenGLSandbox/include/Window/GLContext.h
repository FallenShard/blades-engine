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
    HDC m_deviceContextHandle;
    HGLRC m_resourceContextHandle;
};


#endif