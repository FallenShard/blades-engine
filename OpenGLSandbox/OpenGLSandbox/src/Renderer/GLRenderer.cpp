#include "Renderer/GLRenderer.h"
#include "Window/Event.h"

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
    //prepareOverlapScene();
    prepareMotionScene();
    
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
    
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);

    glClearColor(0.f, 0.f, 0.3f, 1.f);
    glClearDepth(1.0f);
}

void GLRenderer::draw()
{
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //drawTriangleScene();
    //drawPrismScene();
    //drawOverlapScene();
    drawMotionScene();

    VertexBuffer::release(GL_ARRAY_BUFFER);
    VertexArray::release();
    ShaderProgram::release();
}

namespace
{
    bool DepthClamp = false;
}

void GLRenderer::handleEvents(const Event& event)
{
    switch (event.type)
    {
    case Event::KeyPressed:
    {
        switch (event.key.code)
        {
        case Keyboard::Space:
        {
            DepthClamp = !DepthClamp;

            if (DepthClamp == true)
                glEnable(GL_DEPTH_CLAMP);
            else
                glDisable(GL_DEPTH_CLAMP);
        }

        };

        break;
    }


    };
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

    VertexBuffer* positionBuffer = new VertexBuffer(GL_ARRAY_BUFFER, GL_STREAM_DRAW);
    positionBuffer->bind();
    positionBuffer->loadFromFile("res/TrianglePositionData.txt");
    m_vertexBuffers["TrianglePos"] = positionBuffer;

    VertexAttribute* posAttribute = new VertexAttribute("vPosition", 2, GL_FLOAT, GL_FALSE, 0, 0);
    posAttribute->locate(program->getProgramId());
    posAttribute->enable();
    positionBuffer->registerAttributeSize(posAttribute->size);
    m_vertexAttributes.push_back(posAttribute);

    VertexBuffer* colorBuffer = new VertexBuffer(GL_ARRAY_BUFFER, GL_STREAM_DRAW);
    colorBuffer->bind();
    colorBuffer->loadFromFile("res/TriangleColorData.txt");
    m_vertexBuffers["TriangleColor"] = colorBuffer;

    VertexAttribute* colorAttribute = new VertexAttribute("vColor", 4, GL_FLOAT, GL_FALSE, 0, 0);
    colorAttribute->locate(program->getProgramId());
    colorAttribute->enable();
    colorBuffer->registerAttributeSize(colorAttribute->size);
    m_vertexAttributes.push_back(colorAttribute);

    program->getUniformAttribute("time");
    program->getUniformAttribute("loopDuration");
    program->getUniformAttribute("fragLoopDuration");
    program->getUniformAttribute("screenHeight");

    program->use();
    program->setUniformAttribute("screenHeight", 600);

    VertexBuffer::release(GL_ARRAY_BUFFER);
    ShaderProgram::release();
    VertexArray::release();
}

void GLRenderer::drawTriangleScene()
{
    // Triangles
    VertexArray* vArray = m_vertexArrays["Triangles"];
    ShaderProgram* program = m_shaderPrograms["Triangles"];
    VertexBuffer* vBuffer = m_vertexBuffers["TrianglePos"];
    vArray->bind();
    program->use();

    GLsizei vertexCount = vBuffer->getVertexAmount();
    program->setUniformAttribute("time", m_timePassed);
    glDrawArrays(vArray->getPrimitiveType(), 0, vertexCount);

    program->setUniformAttribute("time", m_timePassed + 4.f / 2);
    glDrawArrays(vArray->getPrimitiveType(), 0, vertexCount);
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

    VertexBuffer* prismPosBuffer = new VertexBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    prismPosBuffer->bind();
    prismPosBuffer->loadFromFile("res/PrismPosData.txt");
    m_vertexBuffers["PrismPos"] = prismPosBuffer;

    VertexAttribute* prismPosAttribute = new VertexAttribute("vPosition", 4, GL_FLOAT, GL_FALSE, 0, 0);
    prismPosAttribute->locate(program->getProgramId());
    prismPosAttribute->enable();
    prismPosBuffer->registerAttributeSize(prismPosAttribute->size);
    m_vertexAttributes.push_back(prismPosAttribute);

    VertexBuffer* prismColorBuffer = new VertexBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    prismColorBuffer->bind();
    prismColorBuffer->loadFromFile("res/PrismColorData.txt");
    m_vertexBuffers["PrismColor"] = prismColorBuffer;

    VertexAttribute* prismColorAttribute = new VertexAttribute("vColor", 4, GL_FLOAT, GL_FALSE, 0, 0);
    prismColorAttribute->locate(program->getProgramId());
    prismColorAttribute->enable();
    prismColorBuffer->registerAttributeSize(prismColorAttribute->size);
    m_vertexAttributes.push_back(prismColorAttribute);

    setupDefaultPerspectiveCamera(*program);
    program->getUniformAttribute("offset");
    program->setUniformAttribute("offset", 0.5f, 0.5f);


    VertexBuffer::release(GL_ARRAY_BUFFER);
    ShaderProgram::release();
    VertexArray::release();
}

void GLRenderer::drawPrismScene()
{
    // Prism
    VertexArray* vArray = m_vertexArrays["Prism"];
    ShaderProgram* program = m_shaderPrograms["Prism"];
    VertexBuffer* vBuffer = m_vertexBuffers["PrismPos"];

    vArray->bind();
    program->use();

    glDrawArrays(vArray->getPrimitiveType(), 0, vBuffer->getVertexAmount());
}

void GLRenderer::prepareOverlapScene()
{
    Shader vShader(Shader::Vert);
    vShader.loadFromFile("res/manualOverlap.vert");
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

    GLshort indexData[] =
    {
        // Object 1
        0, 2, 1,
        3, 2, 0,

        4, 5, 6,
        6, 7, 4,

        8, 9, 10,
        11, 13, 12,

        14, 16, 15,
        17, 16, 14,
    };
    
    // Indexed overlap scene
    // Horizontal object
    VertexBuffer* vertexBuffer = new VertexBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    vertexBuffer->bind();
    vertexBuffer->loadFromFile("res/OverlapData.txt");
    m_vertexBuffers["Overlap"] = vertexBuffer;
    VertexBuffer::release(*vertexBuffer);
    
    IndexBuffer* indexBuffer = new IndexBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
    indexBuffer->bind();
    indexBuffer->create(indexData, sizeof(indexData) / sizeof(GLshort));
    m_indexBuffers["Overlap"] = indexBuffer;
    IndexBuffer::release(*indexBuffer);
        
    m_vertexArrays["Overlap"] = new VertexArray(GL_TRIANGLES);
    m_vertexArrays["Overlap"]->bind();
    
    vertexBuffer->bind();

    VertexAttribute* posAttrib = new VertexAttribute("vPosition", 3, GL_FLOAT, GL_FALSE, 0, 0);
    vertexBuffer->registerAttributeSize(posAttrib->size);
    posAttrib->locate(program->getProgramId());
    posAttrib->enable();
    m_vertexAttributes.push_back(posAttrib);
    
    VertexAttribute* colAttrib = new VertexAttribute("vColor", 4, GL_FLOAT, GL_FALSE, 0, -1);
    vertexBuffer->registerAttributeSize(colAttrib->size);
    colAttrib->locate(program->getProgramId());
    GLint colorOffset = sizeof(GLfloat) * 3 * vertexBuffer->getVertexAmount();
    colAttrib->offset = (const GLvoid*)colorOffset;
    colAttrib->enable();
    m_vertexAttributes.push_back(colAttrib);
    indexBuffer->bind();
      
    VertexArray::release();
}

void GLRenderer::drawOverlapScene()
{
    // Overlap Scene
    VertexArray* vArray = m_vertexArrays["Overlap"];
    ShaderProgram* program = m_shaderPrograms["Overlap"];
    
    program->use();

    GLsizei indexCount = m_indexBuffers["Overlap"]->getSize();
    GLsizei vertexCount = m_vertexBuffers["Overlap"]->getVertexAmount();

    vArray->bind();
    program->setUniformAttribute("offset", 0.0f, 0.0f, 0.5f);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, 0);

    program->setUniformAttribute("offset", 0.0f, 0.0f, 0.5f);
    glDrawElementsBaseVertex(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, 0, vertexCount / 2);

    VertexArray::release();
    ShaderProgram::release();
}

namespace
{
    glm::mat4 cameraToClipMatrix;
    glm::mat4 modelToCameraMatrix;

    float CalcFrustumScale(float fovDegrees)
    {
        const float degToRad = 3.14159f / 180.f;
        float fFovRad = fovDegrees * degToRad;
        return 1.0f / tan(fFovRad / 2.0f);
    }

    float fFrustumScale = CalcFrustumScale(45.f);

    struct Instance
    {
        typedef glm::vec3(*OffsetFunc)(float);

        OffsetFunc CalcOffset;

        glm::mat4 ConstructMatrix(float elapsedTime)
        {
            glm::mat4 theMat(1.0f);

            theMat[3] = glm::vec4(CalcOffset(elapsedTime), 1.0f);

            return theMat;
        }
    };
    /*
    Instance g_instanceList[] =
    {
    { StationaryOffset },
    { OvalOffset },
    { BottomCircleOffset }
    };*/

    Instance g_instanceList[3];



    glm::vec3 StationaryOffset(float elapsedTime)
    {
        return glm::vec3(0.f, 0.f, -20.f);
    }

    glm::vec3 OvalOffset(float elapsedTime)
    {
        const float loopDuration = 3.f;
        const float scale = 3.141592f * 2.f / loopDuration;

        float currentTime = fmodf(elapsedTime, loopDuration);

        return glm::vec3(cosf(currentTime * scale) * 4.f,
            sinf(currentTime * scale) * 6.f, -20.f);
    }

    glm::vec3 BottomCircleOffset(float elapsedTime)
    {
        const float loopDuration = 12.f;
        const float scale = 3.141592f * 2.f / loopDuration;

        float currentTime = fmodf(elapsedTime, loopDuration);

        return glm::vec3(cosf(currentTime * scale) * 5.f,
            -3.5f,
            sinf(currentTime * scale) * 5.f - 20.f);
    }


}

void GLRenderer::prepareMotionScene()
{
    Shader vShader(Shader::Vert);
    vShader.loadFromFile("res/motionVert.vert");
    vShader.checkCompileStatus();

    Shader fShader(Shader::Frag);
    fShader.loadFromFile("res/default.frag");
    fShader.checkCompileStatus();

    // Create an accompanying shader program
    ShaderProgram* program = new ShaderProgram();
    m_shaderPrograms["Motion"] = program;
    program->attachShader(vShader);
    program->attachShader(fShader);
    program->link();
    program->checkLinkStatus();
    program->detachShader(vShader);
    program->detachShader(fShader);

    program->getUniformAttribute("position");
    program->getUniformAttribute("color");
    program->getUniformAttribute("modelToCameraMatrix");
    program->getUniformAttribute("cameraToClipMatrix");

    float fzNear = 1.0f; float fzFar = 45.0f;

    cameraToClipMatrix[0].x = fFrustumScale;
    cameraToClipMatrix[1].y = fFrustumScale;
    cameraToClipMatrix[2].z = (fzFar + fzNear) / (fzNear - fzFar);
    cameraToClipMatrix[2].w = -1.0f;
    cameraToClipMatrix[3].z = (2 * fzFar * fzNear) / (fzNear - fzFar);

    program->use();
    program->setUniformAttribute("cameraToClipMatrix", 1, GL_FALSE, glm::value_ptr(cameraToClipMatrix));
    ShaderProgram::release();

    GLfloat vertexData[] =
    {
        +1.0f, +1.0f, +1.0f,
        -1.0f, -1.0f, +1.0f,
        -1.0f, +1.0f, -1.0f,
        +1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        +1.0f, +1.0f, -1.0f,
        +1.0f, -1.0f, +1.0f,
        -1.0f, +1.0f, +1.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.0f, 1.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.0f, 1.0f
    };

    GLshort indexData[] =
    {
        0, 1, 2,
        1, 0, 3,
        2, 3, 0,
        3, 2, 1,

        5, 4, 6,
        4, 5, 7,
        7, 6, 4,
        6, 7, 5,
    };

    VertexBuffer* vBuffer = new VertexBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    vBuffer->bind();
    vBuffer->create(vertexData, sizeof(vertexData) / sizeof(float));
    vBuffer->release(*vBuffer);
    m_vertexBuffers["Motion"] = vBuffer;

    IndexBuffer* indexBuffer = new IndexBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
    indexBuffer->bind();
    indexBuffer->create(indexData, sizeof(indexData) / sizeof(GLshort));
    indexBuffer->release(*indexBuffer);
    m_indexBuffers["Index"] = indexBuffer;

    VertexArray* vArray = new VertexArray(GL_TRIANGLES);
    vArray->bind();
    m_vertexArrays["Motion"] = vArray;

    vBuffer->bind();
    VertexAttribute* posAttrib = new VertexAttribute("vPosition", 3, GL_FLOAT, GL_FALSE, 0, 0);
    vBuffer->registerAttributeSize(posAttrib->size);
    posAttrib->locate(program->getProgramId());
    posAttrib->enable();
    m_vertexAttributes.push_back(posAttrib);

    GLsizei colorOffset = sizeof(GLfloat) * 3 * 8;
    VertexAttribute* colorAttrib = new VertexAttribute("vColor", 4, GL_FLOAT, GL_FALSE, 0, colorOffset);
    vBuffer->registerAttributeSize(colorAttrib->size);
    colorAttrib->locate(program->getProgramId());
    colorAttrib->enable();
    m_vertexAttributes.push_back(colorAttrib);

    indexBuffer->bind();
    
    VertexArray::release();

    g_instanceList[0] = { StationaryOffset };
    g_instanceList[1] = { OvalOffset };
    g_instanceList[2] = { BottomCircleOffset };
}



void GLRenderer::drawMotionScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ShaderProgram* program = m_shaderPrograms["Motion"];
    VertexArray* vArray = m_vertexArrays["Motion"];
    IndexBuffer* iBuffer = m_indexBuffers["Index"];

    program->use();
    vArray->bind();


    for (int iLoop = 0; iLoop < 3; iLoop++)
    {
        Instance &currInst = g_instanceList[iLoop];
        const glm::mat4 &transformMatrix = currInst.ConstructMatrix(m_timePassed);

        program->setUniformAttribute("modelToCameraMatrix", 1, GL_FALSE, glm::value_ptr(transformMatrix));
        glDrawElements(GL_TRIANGLES, iBuffer->getSize(), GL_UNSIGNED_SHORT, 0);
    }
}





