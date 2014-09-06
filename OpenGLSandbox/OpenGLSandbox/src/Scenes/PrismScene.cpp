#include "Scenes/PrismScene.h"

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

PrismScene::PrismScene()
    : m_timePassed(0.f)
{
}

void PrismScene::prepare()
{
    // Create an accompanying shader program
    m_shaderPrograms["Prism"] = std::make_unique<ShaderProgram>();
    ShaderProgram* program = m_shaderPrograms["Prism"].get();
    program->attachShader(Shader::Vert, "manualPerspective.vert");
    program->attachShader(Shader::Frag, "default.frag");
    program->link();
    program->checkLinkStatus();

    // Prism
    m_vertexArrays["Prism"] = std::make_unique<VertexArray>(GL_TRIANGLES);
    VertexArray* vArray = m_vertexArrays["Prism"].get();
    vArray->bind();

    m_vertexBuffers["Prism"] = std::make_unique<VertexBuffer>(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    VertexBuffer* buffer = m_vertexBuffers["Prism"].get();
    buffer->bind();
    buffer->loadFromFile("res/PrismPosData.txt");
    vArray->setVertexCount(buffer->getVertexCount());

    VertexBuffer::AttributeMap attributeMap = buffer->getAttributeMap();
    if (attributeMap.find("Position") != attributeMap.end())
        attributeMap["Position"]->name = "vPosition";
    if (attributeMap.find("Color") != attributeMap.end())
        attributeMap["Color"]->name = "vColor";

    for (auto& attribute : attributeMap)
    {
        attribute.second->locate(program->getProgramId());
        attribute.second->enable();
    }

    setupDefaultPerspectiveCamera(*program);
    program->getUniformAttribute("offset");
    program->use();
    program->setUniformAttribute("offset", 0.5f, 0.5f);

    VertexBuffer::release(*buffer);
    ShaderProgram::release();
    VertexArray::release();
}

void PrismScene::handleEvents(const Event& event)
{
}

void PrismScene::update(float timeDelta)
{
    m_timePassed += timeDelta;
}

void PrismScene::render()
{
    VertexArray* vArray = m_vertexArrays["Prism"].get();
    ShaderProgram* program = m_shaderPrograms["Prism"].get();
    vArray->bind();
    program->use();

    vArray->render();
}