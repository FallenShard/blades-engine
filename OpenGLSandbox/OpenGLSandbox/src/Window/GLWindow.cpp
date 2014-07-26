#include "Window/GLWindow.h"


GLWindow::GLWindow(int x, int y)
    :   m_running(true)
    ,   m_hInstance(GetModuleHandle(NULL))
{
    registerWindowClass();

    m_hWnd = CreateWindow("GLSample", "OpenGL Sample", 
                          WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE,
                          200, 200, x, y,
                          NULL, NULL, m_hInstance, NULL);
    
    m_hDC = GetDC(m_hWnd);
    m_GLContext = new GLContext(m_hDC);

    
}

GLWindow::~GLWindow()
{
    if (m_GLContext)
        delete m_GLContext;

    DestroyWindow(m_hWnd);
}


void GLWindow::registerWindowClass()
{
    m_windowClass.style = CS_OWNDC;
    m_windowClass.lpfnWndProc = (WNDPROC)WndProc;
    m_windowClass.cbClsExtra = 0;
    m_windowClass.cbWndExtra = 0;
    m_windowClass.hInstance = m_hInstance;
    m_windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    m_windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    m_windowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    m_windowClass.lpszMenuName = NULL;
    m_windowClass.lpszClassName = "GLSample";
    RegisterClass(&m_windowClass);
}

HWND GLWindow::getWindowHandle()
{
    return m_hWnd;
}

MSG GLWindow::getMessageHandle()
{
    return m_message;
}

LRESULT CALLBACK GLWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    
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

void GLWindow::run()
{
    GLuint VertexArrayID = 1;
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);
        static const GLfloat g_vertex_buffer_data[] = {
   -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   0.0f,  1.0f, 0.0f,
};

        GLuint vertexbuffer = 10;
 
// Generate 1 buffer, put the resulting identifier in vertexbuffer
glGenBuffers(1, &vertexbuffer);
 
// The following commands will talk about our 'vertexbuffer' buffer
glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
 
// Give our vertices to OpenGL.
glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    while (m_running)
    {
        if (PeekMessage(&m_message, NULL, 0, 0, PM_REMOVE))
        {
            if (m_message.message == WM_QUIT) 
            {
                m_running = false;
            }
            else
            {
                TranslateMessage(&m_message);
                DispatchMessage(&m_message);
            }
        }
        /*
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glPushMatrix();
        glBegin( GL_TRIANGLES );
            glColor3f(1.0f, 0.0f, 0.0f); 
            glVertex2f(0.0f, 1.0f);
            glColor3f(0.0f, 1.0f, 0.0f); 
            glVertex2f(0.87f, -0.5f);
            glColor3f(0.0f, 0.0f, 1.0f); 
            glVertex2f(-0.87f, -0.5f);
        glEnd();
        glPopMatrix();*/
        

glEnableVertexAttribArray(0);
glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
glVertexAttribPointer(
   0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
   3,                  // size
   GL_FLOAT,           // type
   GL_FALSE,           // normalized?
   0,                  // stride
   (void*)0            // array buffer offset
);
 
// Draw the triangle !
glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
 
glDisableVertexAttribArray(0);

        SwapBuffers(m_hDC);
    }  
}
