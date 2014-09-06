#include "Scenes/TriangleScene.h"

TriangleScene::TriangleScene()
    : m_timePassed(0.f)
{
}

void TriangleScene::prepare()
{
    // Create an accompanying shader program
    m_shaderPrograms["Triangles"] = std::make_unique<ShaderProgram>();
    ShaderProgram* program = m_shaderPrograms["Triangles"].get();
    program->attachShader(Shader::Vert, "circularMotion.vert");
    program->attachShader(Shader::Frag, "circularMotion.frag");
    program->link();
    program->checkLinkStatus();

    // Triangles
    m_vertexArrays["Triangles"] = std::make_unique<VertexArray>(GL_TRIANGLES);
    VertexArray* vArray = m_vertexArrays["Triangles"].get();
    vArray->bind();

    m_vertexBuffers["Triangle"] = std::make_unique<VertexBuffer>(GL_ARRAY_BUFFER, GL_STREAM_DRAW);
    VertexBuffer* buffer = m_vertexBuffers["Triangle"].get();
    buffer->bind();
    buffer->loadFromFile("res/TrianglePositionData.txt");
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

    program->getUniformAttribute("time");
    program->getUniformAttribute("loopDuration");
    program->getUniformAttribute("fragLoopDuration");
    program->getUniformAttribute("screenHeight");

    program->use();
    program->setUniformAttribute("screenHeight", 600);

    VertexBuffer::release(*buffer);
    ShaderProgram::release();
    VertexArray::release();
}

void TriangleScene::handleEvents(const Event& event)
{
}

void TriangleScene::update(float timeDelta)
{
    m_timePassed += timeDelta;
}

void TriangleScene::render()
{
    VertexArray* vArray = m_vertexArrays["Triangles"].get();
    ShaderProgram* program = m_shaderPrograms["Triangles"].get();
    vArray->bind();
    program->use();

    program->setUniformAttribute("time", m_timePassed);
    vArray->render();

    program->setUniformAttribute("time", m_timePassed + 4.f / 2);
    vArray->render();
}