#include "Renderer/GLRenderer.h"

namespace
{
    void setupDefaultPerspectiveCamera(ShaderProgram& program)
    {
        program.getUniformAttribute("perspectiveMatrix");

        float theMatrix[16];
        memset(theMatrix, 0, sizeof(float) * 16);

        float fFrustumScale = 1.f, fzFar = 3.f, fzNear = 1.f;

        theMatrix[0] = fFrustumScale;
        theMatrix[5] = fFrustumScale;
        theMatrix[10] = (fzFar + fzNear) / (fzNear - fzFar);
        theMatrix[14] = (2 * fzFar * fzNear) / (fzNear - fzFar);
        theMatrix[11] = -1.0f;

        program.use();
        program.setUniformAttribute("perspectiveMatrix", 1, GL_FALSE, theMatrix);
    }

    GLshort indexData[] =
    {
        0, 2, 1,
        3, 2, 0,

        4, 5, 6,
        6, 7, 4,

        8, 9, 10,
        11, 13, 12,

        14, 16, 15,
        17, 16, 14,
    };
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
    for (auto& program : m_shaderPrograms)
        delete program.second;
    m_shaderPrograms.clear();

    for (auto& vArray : m_vertexArrays)
        delete vArray.second;
    m_vertexArrays.clear();

    for (auto& buffer : m_vertexBuffers)
        delete buffer.second;
    m_vertexBuffers.clear();

    for (auto& buffer : m_indexBuffers)
        delete buffer.second;
    m_indexBuffers.clear();

    for (VertexAttribute* attribute : m_vertexAttributes)
        delete attribute;
    m_vertexAttributes.clear();
}

void GLRenderer::init()
{
    //prepareTriangleScene();
    //preparePrismScene();
    prepareOverlapScene();
    

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    glClearColor(0.f, 0.f, 0.3f, 1.f);
}

void GLRenderer::draw()
{
    glClear(GL_COLOR_BUFFER_BIT);

    //drawTriangleScene();
    //drawPrismScene();
    drawOverlapScene();

    VertexBufferF::release(GL_ARRAY_BUFFER);
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

    if (m_shaderPrograms["Triangles"] != nullptr)
    {
        m_shaderPrograms["Triangles"]->use();
        m_shaderPrograms["Triangles"]->setUniformAttribute("screenHeight", newHeight);
        m_shaderPrograms["Triangles"]->release();
    }
    

    GLint x = static_cast<GLint>((resWidth - newWidth) / 2);
    GLint y = static_cast<GLint>((resHeight - newHeight) / 2);

    glViewport(x, y, static_cast<GLsizei>(newWidth), static_cast<GLsizei>(newHeight));
}

void GLRenderer::prepareTriangleScene()
{
    // Create a vertex shader
    Shader vertShader(Shader::Vert);
    vertShader.loadFromFile("res/circularMotion.vert");
    vertShader.checkCompileStatus();

    // Create a fragment shader
    Shader fragShader(Shader::Frag);
    fragShader.loadFromFile("res/circularMotion.frag");
    fragShader.checkCompileStatus();


    // Create an accompanying shader program
    ShaderProgram* program = new ShaderProgram();
    m_shaderPrograms["Triangles"] = program;
    program->attachShader(vertShader);
    program->attachShader(fragShader);
    program->link();
    program->checkLinkStatus();
    program->detachShader(vertShader);
    program->detachShader(fragShader);

    // Triangles
    m_vertexArrays["Triangles"] = new VertexArray(GL_TRIANGLES);
    m_vertexArrays["Triangles"]->bind();

    VertexBufferF* positionBuffer = new VertexBufferF(GL_ARRAY_BUFFER, GL_STREAM_DRAW);
    positionBuffer->bind();
    positionBuffer->loadFromFile("res/TrianglePositionData.txt", 2);
    m_vertexBuffers["TrianglePos"] = positionBuffer;

    VertexAttribute* posAttribute = new VertexAttribute();
    posAttribute->name = "vPosition";
    posAttribute->location = glGetAttribLocation(program->getProgramId(), posAttribute->name.c_str());
    posAttribute->normalized = GL_FALSE;
    posAttribute->size = 2;
    posAttribute->offset = (GLvoid*)0;
    posAttribute->stride = 0;
    posAttribute->type = GL_FLOAT;
    posAttribute->enable();
    m_vertexAttributes.push_back(posAttribute);

    VertexBufferF* colorBuffer = new VertexBufferF(GL_ARRAY_BUFFER, GL_STREAM_DRAW);
    colorBuffer->bind();
    colorBuffer->loadFromFile("res/TriangleColorData.txt", 4);
    m_vertexBuffers["TriangleColor"] = colorBuffer;

    VertexAttribute* colorAttribute = new VertexAttribute();
    colorAttribute->name = "vColor";
    colorAttribute->location = glGetAttribLocation(program->getProgramId(), colorAttribute->name.c_str());
    colorAttribute->normalized = GL_FALSE;
    colorAttribute->size = 4;
    colorAttribute->offset = (GLvoid*)0;
    colorAttribute->stride = 0;
    colorAttribute->type = GL_FLOAT;
    colorAttribute->enable();
    m_vertexAttributes.push_back(colorAttribute);

    program->getUniformAttribute("time");
    program->getUniformAttribute("loopDuration");
    program->getUniformAttribute("fragLoopDuration");
    program->getUniformAttribute("screenHeight");

    program->use();
    program->setUniformAttribute("screenHeight", 600);

    VertexBufferF::release(GL_ARRAY_BUFFER);
    ShaderProgram::release();
    VertexArray::release();
}

void GLRenderer::drawTriangleScene()
{
    // Triangles
    VertexArray* vArray = m_vertexArrays["Triangles"];
    ShaderProgram* program = m_shaderPrograms["Triangles"];
    vArray->bind();
    program->use();

    program->setUniformAttribute("time", m_timePassed);
    glDrawArrays(vArray->getPrimitiveType(), 0, 3);

    program->setUniformAttribute("time", m_timePassed + 4.f / 2);
    glDrawArrays(vArray->getPrimitiveType(), 0, 3);
}

void GLRenderer::preparePrismScene()
{
    Shader vShader(Shader::Vert);
    vShader.loadFromFile("res/manualPerspective.vert");
    vShader.checkCompileStatus();

    Shader fShader(Shader::Frag);
    fShader.loadFromFile("res/default.frag");
    fShader.checkCompileStatus();

    // Create an accompanying shader program
    ShaderProgram* program = new ShaderProgram();
    m_shaderPrograms["Prism"] = program;
    program->attachShader(vShader);
    program->attachShader(fShader);
    program->link();
    program->checkLinkStatus();
    program->detachShader(vShader);
    program->detachShader(fShader);

    // 3D Prism
    m_vertexArrays["Prism"] = new VertexArray(GL_TRIANGLES);
    m_vertexArrays["Prism"]->bind();

    VertexBufferF* prismPosBuffer = new VertexBufferF(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    prismPosBuffer->bind();
    prismPosBuffer->loadFromFile("res/PrismPosData.txt", 4);
    m_vertexBuffers["PrismPos"] = prismPosBuffer;

    VertexAttribute* prismPosAttribute = new VertexAttribute();
    prismPosAttribute->name = "vPosition";
    prismPosAttribute->location = glGetAttribLocation(program->getProgramId(), prismPosAttribute->name.c_str());
    prismPosAttribute->normalized = GL_FALSE;
    prismPosAttribute->size = 4;
    prismPosAttribute->offset = (GLvoid*)0;
    prismPosAttribute->stride = 0;
    prismPosAttribute->type = GL_FLOAT;
    prismPosAttribute->enable();
    m_vertexAttributes.push_back(prismPosAttribute);

    VertexBufferF* prismColorBuffer = new VertexBufferF(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    prismColorBuffer->bind();
    prismColorBuffer->loadFromFile("res/PrismColorData.txt", 4);
    m_vertexBuffers["PrismColor"] = prismColorBuffer;

    VertexAttribute* prismColorAttribute = new VertexAttribute();
    prismColorAttribute->name = "vColor";
    prismColorAttribute->location = glGetAttribLocation(program->getProgramId(), prismColorAttribute->name.c_str());
    prismColorAttribute->normalized = GL_FALSE;
    prismColorAttribute->size = 4;
    prismColorAttribute->offset = (GLvoid*)0;
    prismColorAttribute->stride = 0;
    prismColorAttribute->type = GL_FLOAT;
    prismColorAttribute->enable();
    m_vertexAttributes.push_back(prismColorAttribute);

    setupDefaultPerspectiveCamera(*program);
    program->getUniformAttribute("offset");
    program->setUniformAttribute("offset", 0.5f, 0.5f);


    VertexBufferF::release(GL_ARRAY_BUFFER);
    ShaderProgram::release();
    VertexArray::release();
}

void GLRenderer::drawPrismScene()
{
    // Prism
    VertexArray* vArray = m_vertexArrays["Prism"];
    ShaderProgram* program = m_shaderPrograms["Prism"];
    vArray->bind();
    program->use();

    glDrawArrays(vArray->getPrimitiveType(), 0, 36);
}

void GLRenderer::prepareOverlapScene()
{
    Shader vShader(Shader::Vert);
    vShader.loadFromFile("res/manualPerspective.vert");
    vShader.checkCompileStatus();

    Shader fShader(Shader::Frag);
    fShader.loadFromFile("res/default.frag");
    fShader.checkCompileStatus();

    // Create an accompanying shader program
    ShaderProgram* program = new ShaderProgram();
    m_shaderPrograms["Overlap"] = program;
    program->attachShader(vShader);
    program->attachShader(fShader);
    program->link();
    program->checkLinkStatus();
    program->detachShader(vShader);
    program->detachShader(fShader);
    
    setupDefaultPerspectiveCamera(*program);
    program->getUniformAttribute("offset");
    
    // Indexed overlap scene
    // Horizontal object
    VertexBufferF* vertexBuffer = new VertexBufferF(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    vertexBuffer->bind();
    vertexBuffer->loadFromFile("res/OverlapData.txt", 3);
    m_vertexBuffers["Overlap"] = vertexBuffer;
    VertexBufferF::release(*vertexBuffer);
    
    VertexBufferS* indexBuffer = new VertexBufferS(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
    indexBuffer->bind();
    indexBuffer->create(indexData, sizeof(indexData), 1);
    m_indexBuffers["Overlap"] = indexBuffer;
    VertexBufferS::release(*indexBuffer);
        
    m_vertexArrays["Overlap"] = new VertexArray(GL_TRIANGLES);
    m_vertexArrays["Overlap"]->bind();

    GLsizei numberOfVertices = vertexBuffer->getSize();
    GLint colorOffset = sizeof(GLfloat) * 3 * numberOfVertices;
    
    vertexBuffer->bind();
    VertexAttribute* posAttrib = new VertexAttribute("vPosition", 3, GL_FLOAT, GL_FALSE, 0, 0);
    posAttrib->locate(program->getProgramId());
    posAttrib->enable();
    
    VertexAttribute* colAttrib = new VertexAttribute("vColor", 4, GL_FLOAT, GL_FALSE, 0, colorOffset);
    colAttrib->locate(program->getProgramId());
    colAttrib->enable();

    indexBuffer->bind();
      
    VertexArray::release();

    m_vertexArrays["Second"] = new VertexArray(GL_TRIANGLES);
    m_vertexArrays["Second"]->bind();

    vertexBuffer->bind();

    GLint posDataOffset = sizeof(float) * 3 * (numberOfVertices / 2);
    colorOffset += sizeof(float) * 4 * (numberOfVertices / 2);

    posAttrib->offset = (const GLvoid*)posDataOffset;
    posAttrib->enable();

    colAttrib->offset = (const GLvoid*)colorOffset;
    colAttrib->enable();
    
    indexBuffer->bind();

    VertexBufferF::release(*vertexBuffer);
    VertexArray::release();
}

void GLRenderer::drawOverlapScene()
{
    // Prism
    VertexArray* vArray = m_vertexArrays["Overlap"];
    VertexArray* vArray2 = m_vertexArrays["Second"];
    ShaderProgram* program = m_shaderPrograms["Overlap"];
    
    program->use();

    GLsizei arrayCount = m_indexBuffers["Overlap"]->getSize();

    vArray->bind();
    program->setUniformAttribute("offset", 0.0f, 0.0f, 0.0f);
    glDrawElements(GL_TRIANGLES, arrayCount, GL_UNSIGNED_SHORT, 0);

    vArray2->bind();
    program->setUniformAttribute("offset", 0.0f, 0.0f, -1.0f);
    glDrawElements(GL_TRIANGLES, arrayCount, GL_UNSIGNED_SHORT, 0);

    VertexArray::release();
    ShaderProgram::release();
}