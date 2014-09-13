#include "Scenes/OverlapScene.h"

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

OverlapScene::OverlapScene()
    : m_timePassed(0.f)
    , m_depthClamp(false)
{
}

void OverlapScene::prepare()
{
    // Create an accompanying shader program
    m_shaderPrograms["Overlap"] = std::make_unique<ShaderProgram>();
    ShaderProgram* program = m_shaderPrograms["Overlap"].get();
    program->attachShader(Shader::Vert, "manualOverlap.vert");
    program->attachShader(Shader::Frag, "default.frag");
    program->link();
    program->checkLinkStatus();

    setupDefaultPerspectiveCamera(*program);
    program->getUniformAttribute("offset");

    // Overlap
    m_vertexArrays["Overlap"] = std::make_unique<VertexArray>();
    VertexArray* vArray = m_vertexArrays["Overlap"].get();
    vArray->bind();

    m_vertexBuffers["Overlap"] = std::make_unique<VertexBuffer>(GL_STATIC_DRAW);
    VertexBuffer* buffer = m_vertexBuffers["Overlap"].get();
    buffer->bind();
    buffer->loadFromFile("res/OverlapData.txt");
    buffer->setDataCountPerVertex(7);
    vArray->setVertexCount(buffer->getVertexCount());

    GLshort indexData[] =
    {
        0, 2, 1,
        3, 2, 0,

        4, 5, 6,
        6, 7, 4,

        8, 9, 10,
        11, 13, 12,

        14, 16, 15,
        17, 16, 14
    };

    m_indexBuffers["Overlap"] = std::make_unique<IndexBuffer>(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
    IndexBuffer* indexBuffer = m_indexBuffers["Overlap"].get();
    indexBuffer->bind();
    indexBuffer->create(indexData, sizeof(indexData) / sizeof(GLshort));
    vArray->attachIndexBuffer(indexBuffer);

    vArray->attachAttribute(VertexAttribute("vPosition", 3, 0, 0));
    vArray->attachAttribute(VertexAttribute("vColor",    4, 0, 3 * sizeof(GLfloat) * buffer->getVertexCount()));
    vArray->enableAttributes(program->getProgramId());

    VertexBuffer::release();
    ShaderProgram::release();
    VertexArray::release();
}

void OverlapScene::handleEvents(const Event& event)
{
    switch (event.type)
    {
    case Event::KeyPressed:
    {
        switch (event.key.code)
        {
        case Keyboard::Space:
        {
            m_depthClamp = !m_depthClamp;

            if (m_depthClamp == true)
                glEnable(GL_DEPTH_CLAMP);
            else
                glDisable(GL_DEPTH_CLAMP);
        }
        };

        break;
    }
    };
}

void OverlapScene::update(float timeDelta)
{
    m_timePassed += timeDelta;
}

void OverlapScene::render()
{
    VertexArray* vArray = m_vertexArrays["Overlap"].get();
    ShaderProgram* program = m_shaderPrograms["Overlap"].get();
    
    program->use();
    
    vArray->bind();

    program->setUniformAttribute("offset", 0.0f, 0.0f, 0.5f);
    vArray->renderIndexed();

    program->setUniformAttribute("offset", 0.0f, 0.0f, 0.5f);
    vArray->renderIndexed(vArray->getVertexCount() / 2);

    VertexArray::release();
    ShaderProgram::release();
}

bool OverlapScene::reshape(int width, int height)
{
    return false;
}