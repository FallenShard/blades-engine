#include "Core/Application.h"

namespace
{
    GLuint VertexArrayID = 15;
        
    static const GLfloat g_vertex_buffer_data[] = { -1.0f, -1.0f, 0.0f,
                                                     1.0f, -1.0f, 0.0f,
                                                     0.0f,  1.0f, 0.0f };

    GLuint vertexbuffer = 20;


    void CheckShader(GLuint id, GLuint type, GLint *ret, const char *onfail)
    {
     //Check if something is wrong with the shader
     switch(type) {
     case(GL_COMPILE_STATUS):
       glGetShaderiv(id, type, ret);
       if(*ret == false){
        int infologLength =  0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infologLength);
        GLchar* buffer = new GLchar[infologLength];
        GLsizei charsWritten = 0;
        glGetShaderInfoLog(id, infologLength, &charsWritten, buffer);
        int x = 5;
       }
       break;
     case(GL_LINK_STATUS):
       glGetProgramiv(id, type, ret);
       if(*ret == false){
        int infologLength =  0;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infologLength);
        GLchar* buffer = new GLchar[infologLength];
        GLsizei charsWritten = 0;
        glGetProgramInfoLog(id, infologLength, &charsWritten, buffer);
        int x = 5;
       }
       break;
     default:
       break;
     };
    }
}

Application::Application()
{
    m_window = new Window(VideoMode(800, 600, 32), "OpenGL Test");
}

Application::~Application()
{
    delete m_window;
    m_window = nullptr;
}

void Application::run()
{
    renderInit();

    while (m_window->isOpen())
    {
        processInput();

        // update();
        render();

        m_window->display();

        Sleep(16);
    }
}

void Application::processInput()
{
    Event event;
    while (m_window->pollEvent(event))
    {
        if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
            m_window->close();
    }
}

void Application::render()
{
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
                       0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                       3,                  // size
                       GL_FLOAT,           // type
                       GL_FALSE,           // normalized?
                       0,                  // stride
                       0                   // array buffer offset
    );
 
    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
 
    glDisableVertexAttribArray(0);
}

void Application::renderInit()
{
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    const GLchar* vertSrc[] = { "#version 420 core \n void main() {gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;}\n" };
    const GLint vertLen[] = { strlen(vertSrc[0]) };

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar* fragSrc[] = {"#version 420 core \n void main() {gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);}\n"};
    const GLint fragLen[] = { strlen(fragSrc[0]) };

    glShaderSource(vertShader, 1, vertSrc, vertLen);
    glShaderSource(fragShader, 1, fragSrc, fragLen);

    GLint statusChecker;
    // Step through the code here, only the last one reports a GL_INVALID_OPERATION
    glCompileShader(vertShader);
    GLenum errorReport = glGetError();

    glCompileShader(fragShader);
    errorReport = glGetError();

    GLint progHandle = glCreateProgram();
    glAttachShader(progHandle, vertShader);
    errorReport = glGetError();
    glAttachShader(progHandle, fragShader);
    errorReport = glGetError();

    glLinkProgram(progHandle);
    CheckShader(vertShader, GL_COMPILE_STATUS, &statusChecker, nullptr);
    CheckShader(fragShader, GL_COMPILE_STATUS, &statusChecker, nullptr);
    CheckShader(progHandle, GL_LINK_STATUS, &statusChecker, nullptr);
    errorReport = glGetError();

    glUseProgram(progHandle);
    errorReport = glGetError();

    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    glGenBuffers(1, &vertexbuffer);
 
    // The following commands will talk to our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
 
    // Give our vertices to OpenGL
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    glClearColor(0.f, 0.f, 0.2f, 1.f);
}

