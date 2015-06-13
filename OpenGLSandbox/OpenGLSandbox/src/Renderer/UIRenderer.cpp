#include "OglWrapper/VertexStreams.h"
#include "Renderer/ShaderManager.h"
#include "Window/Window.h"
#include "Utils/Font.h"
#include "UI/Text.h"

#include "Renderer/UIRenderer.h"

namespace fsi
{

namespace
{
    glm::vec4 colorA;
}

UIRenderer::UIRenderer(std::shared_ptr<Window>& window, ShaderManager* shaderManager)
    : m_textRenderer(std::make_shared<Font>(shaderManager->getProgram("text")))
    , m_textProgram(shaderManager->getProgram("text"))
    , m_simpleProgram(shaderManager->getProgram("simple"))
    , m_aaText(std::make_unique<Text>(m_textRenderer))
    , m_wireframeText(std::make_unique<Text>(m_textRenderer))
    , m_projection(glm::ortho(0.f, static_cast<float>(window->getSize().x), 
                                   static_cast<float>(window->getSize().y), 0.f))
{
    m_aaText->setText("Antialiasing");
    m_wireframeText->setText("Wireframe");

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::vector<float> panelVerts;

    panelVerts.push_back(0.0f);
    panelVerts.push_back(0.0f);
    panelVerts.push_back(0.f);
    panelVerts.push_back(1.f);
    panelVerts.push_back(1.f);
    panelVerts.push_back(1.f);

    panelVerts.push_back(0.0f);
    panelVerts.push_back(0.0f);
    panelVerts.push_back(1.f);
    panelVerts.push_back(1.f);
    panelVerts.push_back(1.f);
    panelVerts.push_back(0.f);

    glCreateVertexArrays(1, &m_panelVao);
    glCreateBuffers(1, &m_panelVbo);

    glNamedBufferData(m_panelVbo, panelVerts.size() * sizeof(GLfloat), panelVerts.data(), GL_STATIC_DRAW);

    glVertexArrayVertexBuffer(m_panelVao, VertexBufferBinding::Slot0, m_panelVbo, 0, 2 * sizeof(GLfloat));
    glVertexArrayAttribBinding(m_panelVao, VertexAttrib::Position, VertexBufferBinding::Slot0);
    glVertexArrayAttribFormat(m_panelVao, VertexAttrib::Position, 2, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(m_panelVao, VertexAttrib::Position);
}

UIRenderer::~UIRenderer()
{
}

bool UIRenderer::handleEvents(const Event& event)
{
    if (event.type == Event::KeyPressed && event.key.code == Keyboard::K)
    {
        colorA += glm::vec4(0.1f, 0.1f, 0.1f, 0.1f);
        return true;
    }

    return false;
}

void UIRenderer::render()
{
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    m_simpleProgram->use();

    glm::vec4 color = glm::vec4(0.1f, 0.1f, 0.1f, 0.8f);
    m_simpleProgram->setUniformAttribute("color", color);

    glm::mat4 baseTrans(glm::translate(glm::vec3(50.f, 50.f, 0.f)));

    glm::mat4 modelMat = baseTrans * glm::scale(glm::vec3(200.f, 400.f, 0.f));
    m_simpleProgram->setUniformAttribute("MVP", m_projection * modelMat);
    
    glBindVertexArray(m_panelVao);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    m_textProgram->use();

    // Antialiasing text
    color = glm::vec4(1.f, 0.8f, 0.f, 1.f);
    m_textProgram->setUniformAttribute("color", color);
    modelMat = baseTrans * glm::translate(glm::vec3(20.f, 20.f, 0.f));
    m_textProgram->setUniformAttribute("MVP", m_projection * modelMat);
    m_aaText->render();

    // Wireframe text
    color = glm::vec4(1.f, 1.0f, 0.f, 1.f);
    m_textProgram->setUniformAttribute("color", color);
    modelMat = baseTrans * glm::translate(glm::vec3(20.f, 60.f, 0.f));
    m_textProgram->setUniformAttribute("MVP", m_projection * modelMat);
    m_wireframeText->render();

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}

void UIRenderer::resize(int width, int height)
{
    m_projection = glm::ortho(0.f, static_cast<float>(width), static_cast<float>(height), 0.f);
}

}
