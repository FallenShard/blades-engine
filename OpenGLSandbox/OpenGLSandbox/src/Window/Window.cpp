#include "Window/Window.h"

namespace
{
    unsigned int windowCount = 0;
    const char* windowClassName = "GL_Window";
}

Window::Window(VideoMode videoMode, const std::string& title, Window::Style style)
    :   m_size(videoMode.getModeWidth(), videoMode.getModeHeight())
    ,   m_windowHandle(nullptr)
    ,   m_deviceContextHandle(nullptr)
    ,   m_GLContext(nullptr)
    ,   m_isOpen(false)
{
    // Register the window class at first call
    if (windowCount == 0)
        registerWindowClass();

    // Compute position and size
    HDC screenDC = GetDC(NULL);
    int left   = 100;
    int top    = 100;
    int width  = videoMode.getModeWidth();
    int height = videoMode.getModeHeight();
    ReleaseDC(NULL, screenDC);

    // Select the appropriate windows style
    DWORD win32Style = WS_VISIBLE;
    if (style == Style::None)
    {
        win32Style |= WS_POPUP;
    }
    else
    {
        if (style & Style::TitleBar)      win32Style |= WS_CAPTION | WS_MINIMIZEBOX;
        if (style & Style::Resizable)     win32Style |= WS_THICKFRAME | WS_MAXIMIZEBOX;
        if (style & Style::CloseControls) win32Style |= WS_SYSMENU;
    }

    // Adjustment for the window client area
    RECT rectangle = {0, 0, width, height};
    AdjustWindowRect(&rectangle, win32Style, false);
    width  = rectangle.right - rectangle.left;
    height = rectangle.bottom - rectangle.top;

    // Create the window
    m_windowHandle = CreateWindow(windowClassName, title.c_str(), win32Style, 
                                  left, top, width, height, NULL, NULL, GetModuleHandle(NULL), this);

    // Create the OpenGL Context
    m_deviceContextHandle = GetDC(m_windowHandle);
    m_GLContext = new GLContext(m_deviceContextHandle);

    // Everything went fine, open the window
    m_isOpen = true;

    // Increment window count
    windowCount++;
}

Window::~Window()
{
    if (m_GLContext != nullptr)
        delete m_GLContext;

    // Destroy the window
    if (m_windowHandle != nullptr)
        DestroyWindow(m_windowHandle);

    // Decrement the window count
    windowCount--;

    // Unregister window class on last destroyed window
    if (windowCount == 0)
        UnregisterClass(windowClassName, GetModuleHandleW(NULL));
}

bool Window::isOpen() const
{
    return m_isOpen;
}

void Window::processMessage()
{
    MSG message;
    while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
    {
        if (message.message == WM_QUIT) 
        {
            m_isOpen = false;
        }
        else
        {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
    }
}

void Window::display()
{
    SwapBuffers(m_deviceContextHandle);
}

void Window::registerWindowClass()
{
    WNDCLASS windowClass;
    windowClass.style         = CS_OWNDC;
    windowClass.lpfnWndProc   = &WndProc;
    windowClass.cbClsExtra    = 0;
    windowClass.cbWndExtra    = 0;
    windowClass.hInstance     = GetModuleHandle(NULL);
    windowClass.hIcon         = NULL;
    windowClass.hCursor       = 0;
    windowClass.hbrBackground = 0;
    windowClass.lpszMenuName  = NULL;
    windowClass.lpszClassName = windowClassName;
    RegisterClass(&windowClass);
}

LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // Windows message processing
    switch (message)
    {   
        case WM_CREATE:
            return 0;
        
        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;
        
        case WM_DESTROY:
            return 0;
        
        case WM_KEYDOWN:
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                    return 0;
            }
            return 0;
    
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);          
    }
}
