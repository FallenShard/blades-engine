#include "Renderer/GLRenderer.h"

namespace
{
    const float PI = 4.f * atanf(1.0f);
    const float fullCircle = 2 * PI;

    float xOffset = 0.f;
    float yOffset = 0.f;

    GLuint offsetLocation;
    GLint timeLocation;
    GLint loopDuration;
}

GLRenderer::GLRenderer()
    : m_aspectRatio(4.f / 3)
    , m_timePassed(0.f)
{
    init();
}

GLRenderer::GLRenderer(int width, int height)
    : m_aspectRatio(static_cast<float>(width) / height)
    , m_timePassed(0.f)
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

    Shader vShader(Shader::Vert);
    vShader.loadFromFile("res/manualPerspective.vert");
    vShader.checkCompileStatus();

    Shader fShader(Shader::Frag);
    fShader.loadFromFile("res/default.frag");
    fShader.checkCompileStatus();

    m_defaultProgram.attachShader(vShader);
    m_defaultProgram.attachShader(fShader);
    m_defaultProgram.link();
    m_defaultProgram.checkLinkStatus();
    m_defaultProgram.detachShader(vShader);
    m_defaultProgram.detachShader(fShader);
}

void GLRenderer::init()
{
    shaderSetup();

    // Triangles
    m_vertexArrays.push_back(new VertexArray(GL_TRIANGLES));
    m_vertexArrays[0]->bind();

    VertexBuffer* positionBuffer = new VertexBuffer(GL_ARRAY_BUFFER, GL_STREAM_DRAW);
    positionBuffer->bind();
    positionBuffer->loadFromFile("res/TrianglePositionData.txt", 2);
    m_vertexBuffers.push_back(positionBuffer);

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

    VertexBuffer* colorBuffer = new VertexBuffer(GL_ARRAY_BUFFER, GL_STREAM_DRAW);
    colorBuffer->bind();
    colorBuffer->loadFromFile("res/TriangleColorData.txt", 4);
    m_vertexBuffers.push_back(colorBuffer);

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

    VertexBuffer::release(GL_ARRAY_BUFFER);
    VertexArray::release();

    m_shaderProgram.getUniformAttribute("time");
    m_shaderProgram.getUniformAttribute("loopDuration");
    m_shaderProgram.getUniformAttribute("fragLoopDuration");
    m_shaderProgram.getUniformAttribute("screenHeight");

    m_shaderProgram.use();
    m_shaderProgram.setUniformAttribute("screenHeight", 600);

    // 3D Prism
    m_vertexArrays.push_back(new VertexArray(GL_TRIANGLES));
    m_vertexArrays[1]->bind();

    VertexBuffer* prismPosBuffer = new VertexBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    prismPosBuffer->bind();
    prismPosBuffer->loadFromFile("res/PrismPosData.txt", 4);
    m_vertexBuffers.push_back(prismPosBuffer);

    VertexAttribute* prismPosAttribute = new VertexAttribute();
    prismPosAttribute->name = "vPosition";
    prismPosAttribute->location = glGetAttribLocation(m_defaultProgram.getProgramId(), prismPosAttribute->name.c_str());
    prismPosAttribute->normalized = GL_FALSE;
    prismPosAttribute->size = 4;
    prismPosAttribute->offset = (GLvoid*)0;
    prismPosAttribute->stride = 0;
    prismPosAttribute->type = GL_FLOAT;
    prismPosAttribute->enable();
    m_vertexAttributes.push_back(prismPosAttribute);

    VertexBuffer* prismColorBuffer = new VertexBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    prismColorBuffer->bind();
    prismColorBuffer->loadFromFile("res/PrismColorData.txt", 4);
    m_vertexBuffers.push_back(prismColorBuffer);

    VertexAttribute* prismColorAttribute = new VertexAttribute();
    prismColorAttribute->name = "vColor";
    prismColorAttribute->location = glGetAttribLocation(m_defaultProgram.getProgramId(), prismColorAttribute->name.c_str());
    prismColorAttribute->normalized = GL_FALSE;
    prismColorAttribute->size = 4;
    prismColorAttribute->offset = (GLvoid*)0;
    prismColorAttribute->stride = 0;
    prismColorAttribute->type = GL_FLOAT;
    prismColorAttribute->enable();
    m_vertexAttributes.push_back(prismColorAttribute);

    m_defaultProgram.getUniformAttribute("offset");
    m_defaultProgram.getUniformAttribute("perspectiveMatrix");

    float theMatrix[16];
    memset(theMatrix, 0, sizeof(float) * 16);

    float fFrustumScale = 1.f, fzFar = 3.f, fzNear = 1.f;

    theMatrix[0] = fFrustumScale;
    theMatrix[5] = fFrustumScale;
    theMatrix[10] = (fzFar + fzNear) / (fzNear - fzFar);
    theMatrix[14] = (2 * fzFar * fzNear) / (fzNear - fzFar);
    theMatrix[11] = -1.0f;

    m_defaultProgram.use();
    m_defaultProgram.setUniformAttribute("offset", 0.5f, 0.5f);
    m_defaultProgram.setUniformAttribute("perspectiveMatrix", 1, GL_FALSE, theMatrix);

    VertexBuffer::release(GL_ARRAY_BUFFER);
    VertexArray::release();
    ShaderProgram::release();

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    glClearColor(0.f, 0.f, 0.3f, 1.f);
}

void GLRenderer::draw()
{
    glClear(GL_COLOR_BUFFER_BIT);

    m_shaderProgram.use();
    m_vertexArrays[0]->bind();
    m_vertexBuffers[0]->bind();

    m_shaderProgram.setUniformAttribute("time", m_timePassed);
    glDrawArrays(m_vertexArrays[0]->getPrimitiveType(), 0, 3);

    m_shaderProgram.setUniformAttribute("time", m_timePassed  + 4.f / 2);
    glDrawArrays(m_vertexArrays[0]->getPrimitiveType(), 0, 3);

    

    m_defaultProgram.use();
    m_vertexArrays[1]->bind();
    m_vertexBuffers[2]->bind();

    glDrawArrays(m_vertexArrays[1]->getPrimitiveType(), 0, 36);


    VertexBuffer::release(GL_ARRAY_BUFFER);
    VertexArray::release();
    ShaderProgram::release();
}

void GLRenderer::update(float timeDelta)
{
    m_timePassed += timeDelta;
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

    m_shaderProgram.setUniformAttribute("screenHeight", newHeight);

    GLint x = static_cast<GLint>((resWidth - newWidth) / 2);
    GLint y = static_cast<GLint>((resHeight - newHeight) / 2);

    glViewport(x, y, static_cast<GLsizei>(newWidth), static_cast<GLsizei>(newHeight));
}