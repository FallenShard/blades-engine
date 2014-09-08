#include "Scenes/PrismScene.h"

namespace
{
    float theMatrix[16];

    void setupDefaultPerspectiveCamera(ShaderProgram& program)
    {
        program.getUniformAttribute("perspectiveMatrix");

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

    float frustumScale = 1.f;
    float eyeOffset[3];
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
    program->getUniformAttribute("frustumScale");
    program->getUniformAttribute("eyeOffset");
    program->use();
    program->setUniformAttribute("offset", 0.5f, 0.5f);
    program->setUniformAttribute("frustumScale", frustumScale);
    program->setUniformAttribute("eyeOffset", +0.5f, -0.0f, -1.f);

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
            frustumScale += 0.1f;
            break;

        case Keyboard::D:
            frustumScale -= 0.1f;
            break;
        };
        break;
    };
}

void PrismScene::update(float timeDelta)
{
    m_timePassed += timeDelta;
}

void PrismScene::render()
{
    VertexArray* vArray = m_vertexArrays["Prism"].get();
    ShaderProgram* program = m_shaderPrograms["Prism"].get();
    program->use();
    program->setUniformAttribute("frustumScale", frustumScale);
    program->setUniformAttribute("perspectiveMatrix", 1, GL_FALSE, theMatrix);
    vArray->bind();
    

    vArray->render();
}

bool PrismScene::reshape(int width, int height)
{
   /* theMatrix[0] = frustumScale / (width / (float)height);
    theMatrix[5] = frustumScale;

    glViewport(0, 0, (GLsizei)width, (GLsizei)height);*/

    return false;
}