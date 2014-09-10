#include "Scenes/PrismScene.h"

namespace
{
    float theMatrix[16];

    void setupDefaultPerspectiveCamera(ShaderProgram& program)
    {
        program.getUniformAttribute("perspectiveMatrix");

        memset(theMatrix, 0, sizeof(float) * 16);

        float frustumScale = 1.f, zNear = 1.f, zFar = 3.f;

        theMatrix[0] = frustumScale;
        theMatrix[5] = frustumScale;
        theMatrix[10] = (zNear + zFar) / (zNear - zFar);
        theMatrix[14] = (2 * zNear * zFar) / (zNear - zFar);
        theMatrix[11] = -1.0f;

        program.use();
        program.setUniformAttribute("perspectiveMatrix", 1, GL_FALSE, theMatrix);
    }

    float frustumScale = 1.f;
    float zNear = 1.f;
    float zFar = 3.f;

    bool aPressed = false;
    bool dPressed = false;
    bool sPressed = false;
    bool wPressed = false;
    bool fPressed = false;
    bool gPressed = false;
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
    buffer->loadFromFile("res/PrismData.txt");
    vArray->setVertexCount(buffer->getVertexCount());

    VertexBuffer::AttributeMap attributeMap = buffer->getAttributeMap();
    for (auto& attribute : attributeMap)
        attribute.second->enable(program->getProgramId());

    setupDefaultPerspectiveCamera(*program);

    VertexBuffer::release(*buffer);
    ShaderProgram::release();
    VertexArray::release();
}

void PrismScene::handleEvents(const Event& event)
{
    switch (event.type)
    {
    case Event::KeyPressed:
        switch (event.key.code)
        {
        case Keyboard::A:
            aPressed = true;
            break;

        case Keyboard::D:
            dPressed = true;
            break;

        case Keyboard::W:
            wPressed = true;
            break;

        case Keyboard::S:
            sPressed = true;
            break;

        case Keyboard::F:
            fPressed = true;
            break;

        case Keyboard::G:
            gPressed = true;
            break;
        };
        break;

    case Event::KeyReleased:
        switch (event.key.code)
        {
        case Keyboard::A:
            aPressed = false;
            break;

        case Keyboard::D:
            dPressed = false;
            break;

        case Keyboard::W:
            wPressed = false;
            break;

        case Keyboard::S:
            sPressed = false;
            break;

        case Keyboard::F:
            fPressed = false;
            break;

        case Keyboard::G:
            gPressed = false;
            break;
        };
        break;
    };
}

void PrismScene::update(float timeDelta)
{
    m_timePassed += timeDelta;

    if (aPressed)
        theMatrix[12] -= 0.02f;
    if (dPressed)
        theMatrix[12] += 0.02f;
    if (wPressed)
        theMatrix[13] += 0.02f;
    if (sPressed)
        theMatrix[13] -= 0.02f;
    if (fPressed)
        theMatrix[11] -= 0.02f;
    if (gPressed)
        theMatrix[11] += 0.02f;
}

void PrismScene::render()
{
    VertexArray* vArray = m_vertexArrays["Prism"].get();
    ShaderProgram* program = m_shaderPrograms["Prism"].get();
    program->use();
    vArray->bind();

    program->setUniformAttribute("perspectiveMatrix", 1, GL_FALSE, theMatrix);

    vArray->render();
}

bool PrismScene::reshape(int width, int height)
{
    theMatrix[0] = frustumScale / (width / (float)height);
    theMatrix[5] = frustumScale;

    ShaderProgram* program = m_shaderPrograms["Prism"].get();
    program->use();
    program->setUniformAttribute("perspectiveMatrix", 1, GL_FALSE, theMatrix);

    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    return true;
}