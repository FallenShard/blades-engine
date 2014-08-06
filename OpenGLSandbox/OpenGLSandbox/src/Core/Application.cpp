#include "Core/Application.h"
#include <fstream>
#include "Renderer/Shader.h"

namespace
{
    const float vertexPositions[] = {
        0.75f, 0.75f, 0.0f, 1.0f,
        0.75f, -0.75f, 0.0f, 1.0f,
        -0.75f, -0.75f, 0.0f, 1.0f,
    };

    GLuint positionBufferObject;
    GLuint vao;
    

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
        delete[] buffer;
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
        delete[] buffer;
       }
       break;
     default:
       break;
     };
    }

    GLuint progHandle;
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
        if (event.type == Event::KeyPressed && event.key.code == Keyboard::A)
            backColor = 1;
        if (event.type == Event::KeyPressed && event.key.code == Keyboard::D)
            backColor = 0;
    }
}

void Application::render()
{
    GLenum errorReport = 0;
    if (backColor == 1)
        glClearColor(1.f, 0.f, 0.0f, 1.f);
    else
        glClearColor(0.f, 0.f, 1.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glUseProgram(progHandle);
    errorReport = glGetError();

    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
    errorReport = glGetError();
    glEnableVertexAttribArray(vao);
    errorReport = glGetError();
    glVertexAttribPointer(vao, 4, GL_FLOAT, GL_FALSE, 0, 0);
    errorReport = glGetError();
    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
    errorReport = glGetError();
    glDisableVertexAttribArray(0);
    errorReport = glGetError();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    errorReport = glGetError();
    glUseProgram(0);
    errorReport = glGetError();
}

void Application::renderInit()
{
    Shader vertShader(Shader::Vert);
    vertShader.loadFromFile("res/default.vert");
    vertShader.checkCompileError();

    Shader fragShader(Shader::Frag);
    fragShader.loadFromFile("res/default.frag");
    fragShader.checkCompileError();

    progHandle = glCreateProgram();
    glAttachShader(progHandle, vertShader.getShaderIdentifier());
    glAttachShader(progHandle, fragShader.getShaderIdentifier());

    GLint statusChecker = 0;
    glLinkProgram(progHandle);
    CheckShader(progHandle, GL_LINK_STATUS, &statusChecker, nullptr);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);


    glGenBuffers(1, &positionBufferObject);

    vao = glGetAttribLocation(progHandle, "vertexPosition");

    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

