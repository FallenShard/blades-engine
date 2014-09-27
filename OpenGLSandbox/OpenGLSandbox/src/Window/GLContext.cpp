#include "Window/GLContext.h"

GLContext::GLContext(HDC deviceContextHandle)
    :   m_deviceContextHandle(deviceContextHandle)
{
    initialize();
}

GLContext::~GLContext()
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(m_resourceContextHandle);
}

void GLContext::initialize()
{
    // Create a new pixel format descriptor
    PIXELFORMATDESCRIPTOR pfd;
    int format;

    ZeroMemory(&pfd, sizeof(pfd));
    pfd.nSize        = sizeof(pfd);
    pfd.nVersion     = 1;
    pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType   = PFD_TYPE_RGBA;
    pfd.iLayerType   = PFD_MAIN_PLANE;
    pfd.cColorBits   = 32;
    pfd.cDepthBits   = 24;
    pfd.cStencilBits = 8;
    pfd.cAlphaBits   = 8;

    format = ChoosePixelFormat(m_deviceContextHandle, &pfd);

    int errorReport = SetPixelFormat(m_deviceContextHandle, format, &pfd);

    // Create an OpenGL resource context to query version and such
    HGLRC dummyContextHandle = wglCreateContext(m_deviceContextHandle);
    
    // Enable this context on the current thread and handle
    wglMakeCurrent(m_deviceContextHandle, dummyContextHandle);

    // Initialize GL3W
    gl3wInit();

    // Get the current version on the device
    
    const GLubyte* version = glGetString(GL_VERSION);

    m_majorVersion = version[0] - '0';
    m_minorVersion = version[2] - '0';
    

    // Set up attributes for the actual context
    int attributes[] = 
    {
         WGL_CONTEXT_MAJOR_VERSION_ARB, m_majorVersion,
         WGL_CONTEXT_MINOR_VERSION_ARB, m_minorVersion,
         WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
         0
    };

    // GL3W loads only core profile, so obtain this wgl extension manually
    auto wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

    // Disable and delete dummy context on the curent thread and window
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(dummyContextHandle);
    
    // Open a new resource context with specified attributes
    m_resourceContextHandle = wglCreateContextAttribsARB(m_deviceContextHandle, 0, attributes);

    // Activate the new core-profile context
    wglMakeCurrent(m_deviceContextHandle, m_resourceContextHandle);

    version = glGetString(GL_SHADING_LANGUAGE_VERSION);
}
