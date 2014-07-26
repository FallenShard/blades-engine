#include "Window/GLContext.h"

GLContext::GLContext(HDC deviceContextHandle)
    :   m_deviceContextHandle(deviceContextHandle)
{
    // Create a new pixel format descriptor
    PIXELFORMATDESCRIPTOR pfd;
    int format;

    ZeroMemory(&pfd, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    format = ChoosePixelFormat(m_deviceContextHandle, &pfd);

    SetPixelFormat(m_deviceContextHandle, format, &pfd);

    // Create an OpenGL resource context
    m_resourceContextHandle = wglCreateContext(m_deviceContextHandle);
    
    // Enable this context on the current thread
    wglMakeCurrent(m_deviceContextHandle, m_resourceContextHandle);
    
    // Initialize GLEW
    glewInit();
}

GLContext::~GLContext()
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(m_resourceContextHandle);
}
