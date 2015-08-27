#include <iostream>

#include "Renderer/GLRenderer.h"
#include "Renderer/Technique.h"
#include "Input/Event.h"
#include "Utils/Math.h"

#include "GUI/CheckBox.h"
#include "GUI/Text.h"

namespace fsi
{
    namespace
    {
        GLfloat CheckBoxSize = 24.f;
    }

    namespace gui
    {
        CheckBox::CheckBox(std::shared_ptr<Font>& font, GLRenderer* renderer, bool isChecked)
            : Component({ 0.f, 0.f, 0.f }, {1.f, 1.f})
            , m_isChecked(isChecked)
            , m_callback(nullptr)
            , m_text(std::make_shared<Text>(font, renderer))
            , m_mouseAreaSize(glm::vec2(CheckBoxSize, CheckBoxSize))
            , m_renderer(renderer)
            , m_technique(std::make_unique<Technique>(renderer->getTechniqueCache()->getProgram("gui")))
            , m_vao(0)
            , m_vbo(0)
            , m_ibo(0)
        {
            std::vector<GLfloat> vertices = generateVertices();
            std::vector<GLushort> indices = generateIndices();

            auto bufferMgr = renderer->getDeviceBufferManager();
            m_vbo = bufferMgr->allocate(vertices.size() * sizeof(GLfloat), GL_MAP_WRITE_BIT);
            bufferMgr->update(m_vbo, vertices);
            m_ibo = bufferMgr->allocate(indices.size() * sizeof(GLushort), GL_MAP_WRITE_BIT);
            bufferMgr->update(m_ibo, indices);

            VertexLayout layout;
            layout.indexBuffer = m_ibo;
            layout.vertexBuffers.emplace_back(0, BufferDescriptor{ m_vbo, 0, 4 * sizeof(GLfloat) });
            layout.attributes.emplace_back(0, AttributeFormat{ VertexAttrib::Position, 4, 0 });
            m_vao = renderer->getVertexAssembly()->createInputState(layout);

            auto texMgr = renderer->getTextureManager();
            auto tex = texMgr->loadTexture("checkbox.png", 2, InternalFormat::RGBA8, BaseFormat::RGBA);
            auto sampler = texMgr->getSamplerPreset(TextureManager::LinearClamp);
            auto texInfo = texMgr->createTextureInfo(tex, sampler);

            m_technique->setUniformAttribute("texture", texInfo.unit);
            setState(m_isChecked);

            setText("Example CheckBox 1");
            m_text->setPosition({ 30.f, 20.f, 0.f });
            m_text->setColor({ 0.f, 1.f, 1.f, 1.f });
            addComponent(m_text);
        }

        CheckBox::~CheckBox()
        {
        }

        void CheckBox::setState(bool isChecked)
        {
            float texOffset = isChecked ? 0.f : 0.5f;
            
            m_technique->setUniformAttribute("offset", glm::vec2(texOffset, 0.f));
        }

        std::vector<GLfloat> CheckBox::generateVertices()
        {
            std::vector<GLfloat> vertices;

            vertices.push_back(0.f);
            vertices.push_back(0.f);
            vertices.push_back(0.f);
            vertices.push_back(0.f);

            vertices.push_back(CheckBoxSize);
            vertices.push_back(0.f);
            vertices.push_back(0.49f);
            vertices.push_back(0.0f);

            vertices.push_back(CheckBoxSize);
            vertices.push_back(CheckBoxSize);
            vertices.push_back(0.49f);
            vertices.push_back(1.f);

            vertices.push_back(0.f);
            vertices.push_back(CheckBoxSize);
            vertices.push_back(0.f);
            vertices.push_back(1.f);

            return vertices;
        }

        std::vector<GLushort> CheckBox::generateIndices()
        {
            std::vector<GLushort> indices;
            
            indices.push_back(0);
            indices.push_back(2);
            indices.push_back(1);

            indices.push_back(0);
            indices.push_back(3);
            indices.push_back(2);

            return indices;
        }

        void CheckBox::render(const glm::mat4& P)
        {
            glUseProgram(m_technique->getRawHandle());
            m_technique->setUniformAttribute("MVP", P * m_modelMat);
            setState(m_isChecked);

            glBindVertexArray(m_vao);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

            m_text->render(P);
        }

        void CheckBox::setText(const std::string& text)
        {
            m_text->setText(text);
            m_mouseAreaSize = glm::vec2(30.f + m_text->getTextSize().x, CheckBoxSize);
        }

        glm::vec4 CheckBox::getBounds() const
        {
            glm::vec3 absPos = getAbsolutePosition();
            return glm::vec4(absPos.x, absPos.y, absPos.x + m_mouseAreaSize.x, + absPos.y + m_mouseAreaSize.y);
        }

        void CheckBox::setCallback(std::function<void(bool)> callback)
        {
            m_callback = callback;
        }

        void CheckBox::handleEvents(const Event& event)
        {
            if (event.type == Event::MouseButtonReleased)
            {
                int x = event.mouseButton.x;
                int y = event.mouseButton.y;

                m_isChecked = !m_isChecked;
                setState(m_isChecked);

                if (m_callback)
                    m_callback(m_isChecked);
            }
        }
    }
}