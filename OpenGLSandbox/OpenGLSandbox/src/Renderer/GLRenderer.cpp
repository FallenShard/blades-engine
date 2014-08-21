#include "Renderer/GLRenderer.h"

namespace
{
    const float PI = 4.f * atanf(1.0f);
    const float fullCircle = 2 * PI;

    GLfloat vertexPositions[] =
    {
        0.0f, 0.5f,
        0.5f, -0.366f, 
        -0.5f, -0.366f
    };

    GLfloat vertexColors[] =
    {
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f
    };

    float xOffset = 0.f;
    float yOffset = 0.f;

    GLuint offsetLocation;
    GLint timeLocation;
    GLint loopDuration;
}

GLRenderer::GLRenderer()
    : m_aspectRatio(4.f / 3)
{
    init();
}

GLRenderer::GLRenderer(int width, int height)
    : m_aspectRatio(static_cast<float>(width) / height)
{
    init();
}

GLRenderer::~GLRenderer()
{
    for (VertexArray* array : m_vertexArrays)
        delete array;
    m_vertexArrays.clear();

    for (VertexBuffer* buffer : m_vertexBuffers)
        delete buffer;
    m_vertexBuffers.clear();

    for (VertexAttribute* attribute : m_vertexAttributes)
        delete attribute;
    m_vertexAttributes.clear();
}

void GLRenderer::shaderSetup()
{
    // Default vertex shader
    Shader vertShader(Shader::Vert);
    vertShader.loadFromFile("res/circularMotion.vert");
    vertShader.checkCompileStatus();
    
    // Default fragment shader
    Shader fragShader(Shader::Frag);
    fragShader.loadFromFile("res/circularMotion.frag");
    fragShader.checkCompileStatus();

    
    // Create an accompanying shader program
    m_shaderProgram.attachShader(vertShader);
    m_shaderProgram.attachShader(fragShader);
    m_shaderProgram.link();
    m_shaderProgram.checkLinkStatus();
    m_shaderProgram.detachShader(vertShader);
    m_shaderProgram.detachShader(fragShader);
}

void GLRenderer::init()
{
    shaderSetup();

    m_vertexArrays.push_back(new VertexArray());
    m_vertexArrays[0]->bind();

    VertexBuffer* positionBuffer = new VertexBuffer(GL_ARRAY_BUFFER, GL_STREAM_DRAW);
    VertexBuffer* colorBuffer = new VertexBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    m_vertexBuffers.push_back(positionBuffer);
    m_vertexBuffers.push_back(colorBuffer);
    m_vertexArrays[0]->attachBuffers(m_vertexBuffers);

    positionBuffer->bind();
    positionBuffer->loadFromFile("res/TrianglePositionData.txt", 2);

    VertexAttribute* posAttribute = new VertexAttribute();
    posAttribute->name = "vPosition";
    posAttribute->location = glGetAttribLocation(m_shaderProgram.getProgramId(), posAttribute->name.c_str());
    posAttribute->normalized = GL_FALSE;
    posAttribute->size = 2;
    posAttribute->offset = (GLvoid*)0;
    posAttribute->stride = 0;
    posAttribute->type = GL_FLOAT;

    posAttribute->enable();

    m_vertexAttributes.push_back(posAttribute);

    colorBuffer->bind();
    colorBuffer->loadFromFile("res/TriangleColorData.txt", 4);

    VertexAttribute* colorAttribute = new VertexAttribute();
    colorAttribute->name = "vColor";
    colorAttribute->location = glGetAttribLocation(m_shaderProgram.getProgramId(), colorAttribute->name.c_str());
    colorAttribute->normalized = GL_FALSE;
    colorAttribute->size = 4;
    colorAttribute->offset = (GLvoid*)0;
    colorAttribute->stride = 0;
    colorAttribute->type = GL_FLOAT;

    colorAttribute->enable();

    m_vertexAttributes.push_back(colorAttribute);

    timeLocation = glGetUniformLocation(m_shaderProgram.getProgramId(), "time");
    loopDuration = glGetUniformLocation(m_shaderProgram.getProgramId(), "loopDuration");


    m_shaderProgram.use();

    glClearColor(0.f, 0.f, 0.3f, 1.f);
}

void GLRenderer::draw()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void GLRenderer::update(float timeDelta)
{
    static float timePassed = 0.0f;

    glUniform1f(timeLocation, timePassed);

    timePassed += timeDelta;
}

void GLRenderer::resize(int width, int height)
{
    float resWidth = static_cast<float>(width);
    float resHeight = static_cast<float>(height);
    float newRatio = resWidth / resHeight;

    float newWidth;
    float newHeight;

    if (newRatio > m_aspectRatio)
    {
        newWidth = resHeight * m_aspectRatio;
        newHeight = resHeight;
    }
    else
    {
        newHeight = resWidth / m_aspectRatio;
        newWidth = resWidth;
    }

    GLint x = static_cast<GLint>((resWidth - newWidth) / 2);
    GLint y = static_cast<GLint>((resHeight - newHeight) / 2);

    glViewport(x, y, static_cast<GLsizei>(newWidth), static_cast<GLsizei>(newHeight));
}