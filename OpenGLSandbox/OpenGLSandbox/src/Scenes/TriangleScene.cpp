#include "Scenes/TriangleScene.h"

TriangleScene::TriangleScene()
    : m_timePassed(0.f)
{
}

namespace
{
    GLuint vao, vbo;

    GLfloat data[] = { 0.0f, 0.5f,

        0.5f, -0.366f,

        -0.5f, -0.366f };

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
    m_vertexArrays["Triangles"] = std::make_unique<VertexArray>();
    VertexArray* vArray = m_vertexArrays["Triangles"].get();
    vArray->bind();
    
    m_vertexBuffers["Triangles"] = std::make_unique<VertexBuffer>(GL_STATIC_DRAW);
    VertexBuffer* buffer = m_vertexBuffers["Triangles"].get();
    buffer->bind();
    buffer->loadFromFile("res/TrianglePosData.txt");
    buffer->setDataCountPerVertex(2);
    vArray->setVertexCount(buffer->getVertexCount());

    vArray->attachAttribute(VertexAttribute("vPosition", 2, 0, 0));
    vArray->enableAttributes(program->getProgramId());

    program->getUniformAttribute("time");
    program->getUniformAttribute("loopDuration");
    program->getUniformAttribute("fragLoopDuration");
    program->getUniformAttribute("screenHeight");

    program->use();
    program->setUniformAttribute("screenHeight", 600);

    VertexArray::release();
    VertexBuffer::release();
    ShaderProgram::release();
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

    program->use();
    vArray->bind();

    program->setUniformAttribute("time", m_timePassed);
    vArray->render();

    program->setUniformAttribute("time", m_timePassed + 4.f / 2);
    vArray->render();
}

bool TriangleScene::reshape(int width, int height)
{
    return false;
}