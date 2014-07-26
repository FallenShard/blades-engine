#include "Core/Application.h"

namespace
{
            GLuint VertexArrayID = 1;
        
        static const GLfloat g_vertex_buffer_data[] = {
   -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   0.0f,  1.0f, 0.0f,
};

        GLuint vertexbuffer = 10;
 
// Generate 1 buffer, put the resulting identifier in vertexbuffer

}

Application::Application()
{
    initialize();
}

Application::~Application()
{
    if (m_window != nullptr)
        delete m_window;
}

void Application::run()
{
    glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);
    glGenBuffers(1, &vertexbuffer);
 
// The following commands will talk about our 'vertexbuffer' buffer
glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
 
// Give our vertices to OpenGL.
glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    while (m_window->isOpen())
    {
        // processInput();
        m_window->processMessage();
        // update();
        render();

        m_window->display();
    }
}

void Application::initialize()
{
    m_window = new Window(VideoMode(800, 600, 32), "OpenGL Test", Window::Style::Default);
}

void Application::render()
{/*
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
}