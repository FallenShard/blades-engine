#include <iostream>

#include "Window/Window.h"

#include "Renderer/GLContext.h"

namespace fsi
{
    GLContext::GLContext(std::shared_ptr<Window>& window)
        : m_window(window)
        , m_resourceContext(nullptr)
    {
        PIXELFORMATDESCRIPTOR pfd;
        ZeroMemory(&pfd, sizeof(pfd));
        pfd.nSize = sizeof(pfd);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.iLayerType = PFD_MAIN_PLANE;
        pfd.cColorBits = 32;
        pfd.cDepthBits = 24;
        pfd.cStencilBits = 8;

        HDC deviceContext = GetDC(m_window->getWindowHandle());
        int format = ChoosePixelFormat(deviceContext, &pfd);
        SetPixelFormat(deviceContext, format, &pfd);

        // Create an OpenGL resource context to query version and such
        HGLRC dummyContext = wglCreateContext(deviceContext);

        // Enable this context on the current thread and handle
        wglMakeCurrent(deviceContext, dummyContext);

        // Initialize GL3W
        gl3wInit();

        // Get the current version on the device
        const GLubyte* version = glGetString(GL_VERSION);

        int majorVersion = version[0] - '0';
        int minorVersion = version[2] - '0';

        // Set up attributes for the actual context
        int attributes[] =
        {
            WGL_CONTEXT_MAJOR_VERSION_ARB, majorVersion,
            WGL_CONTEXT_MINOR_VERSION_ARB, minorVersion,
            WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
            0
        };

        // GL3W loads only core profile, so obtain this wgl extension manually
        auto wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

        // Disable and delete dummy context on the curent thread and window
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(dummyContext);

        // Open a new resource context with specified attributes
        m_resourceContext = wglCreateContextAttribsARB(deviceContext, 0, attributes);

        // Activate the new core-profile context
        wglMakeCurrent(deviceContext, m_resourceContext);

#ifdef _DEBUG
        std::cout << "Vendor:\t\t" << glGetString(GL_VENDOR) << std::endl;
        std::cout << "Renderer:\t" << glGetString(GL_RENDERER) << std::endl;
        std::cout << "OpenGL:\t\t" << glGetString(GL_VERSION) << std::endl;
        std::cout << "GLSL:\t\t" << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
#endif
    }

    GLContext::~GLContext()
    {
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(m_resourceContext);
    }

    void GLContext::attachWindow(std::shared_ptr<Window>& window)
    {
        HDC deviceContext = GetDC(m_window->getWindowHandle());
        int format = GetPixelFormat(deviceContext);

        PIXELFORMATDESCRIPTOR pfd;
        ZeroMemory(&pfd, sizeof(pfd));
        DescribePixelFormat(deviceContext, format, sizeof(pfd), &pfd);
        SetPixelFormat(GetDC(window->getWindowHandle()), format, &pfd);
    }
}
