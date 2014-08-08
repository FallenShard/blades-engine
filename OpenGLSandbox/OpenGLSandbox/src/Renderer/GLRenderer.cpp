#include "Renderer/GLRenderer.h"

namespace
{
    const float vertexPositions[] = 
    {
         0.75f,  0.75f, 0.0f, 1.0f,
         0.75f, -0.75f, 0.0f, 1.0f,
        -0.75f, -0.75f, 0.0f, 1.0f,
    };

    GLuint positionBufferObject;
    GLuint vao;
}

GLRenderer::GLRenderer()
{
    init();
}

void GLRenderer::init()
{
    // Default vertex shader
    Shader vertShader(Shader::Vert);
    vertShader.loadFromFile("res/default.vert");
    vertShader.checkCompileStatus();

    // Default fragment shader
    Shader fragShader(Shader::Frag);
    fragShader.loadFromFile("res/default.frag");
    fragShader.checkCompileStatus();

    // 
    m_shaderProgram.attachShader(vertShader);
    m_shaderProgram.attachShader(fragShader);
    m_shaderProgram.link();
    m_shaderProgram.checkLinkStatus();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);


    glGenBuffers(1, &positionBufferObject);

    vao = glGetAttribLocation(m_shaderProgram.getProgramId(), "vertexPosition");

    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


}

void GLRenderer::draw()
{
    //glClearColor(0.f, 0.f, 1.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_shaderProgram.getProgramId());

    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
    glEnableVertexAttribArray(vao);

    glVertexAttribPointer(vao, 4, GL_FLOAT, GL_FALSE, 0, 0);

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle

    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glUseProgram(0);

}