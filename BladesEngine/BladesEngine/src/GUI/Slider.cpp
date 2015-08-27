#include <iostream>

#include "Renderer/GLRenderer.h"
#include "Renderer/Technique.h"
#include "Input/Event.h"
#include "Utils/Math.h"
#include "Utils/StringConversion.h"

#include "GUI/Slider.h"
#include "GUI/Text.h"

namespace fsi
{
    namespace
    {
        float SliderWidth = 10.f;
        float SliderHeight = 15.f;
        float BarWidth = 100.f;

        glm::vec2 m_originOffset(SliderWidth / 2, SliderHeight / 2);
    }

    namespace gui
    {
        Slider::Slider(std::shared_ptr<Font>& font, GLRenderer* renderer, int values, int defaultValue)
            : m_values(values)
            , m_currValue(defaultValue)
            , m_callback(nullptr)
            , m_color({ 0.f, 1.f, 1.f, 1.f })
            , m_sliderSize(SliderWidth, SliderHeight)
            , m_isDragged(false)
            , m_barVao(0)
            , m_barVbo(0)
            , m_sliderVao(0)
            , m_sliderVbo(0)
            , m_sliderIbo(0)
            , m_technique(std::make_unique<Technique>(renderer->getTechniqueCache()->getProgram("simple")))
        {
            std::vector<GLfloat> sliderVerts = generateSliderVertices(SliderWidth, SliderHeight);
            std::vector<GLushort> sliderIndices = generateSliderIndices();
            std::vector<GLfloat> barVerts = generateBarVertices();
            
            auto bufferMgr = renderer->getDeviceBufferManager();
            m_sliderVbo = bufferMgr->allocate(sliderVerts.size() * sizeof(GLfloat), GL_MAP_WRITE_BIT);
            bufferMgr->update(m_sliderVbo, sliderVerts);
            m_sliderIbo = bufferMgr->allocate(sliderIndices.size() * sizeof(GLushort), GL_MAP_WRITE_BIT);
            bufferMgr->update(m_sliderIbo, sliderIndices);
            m_barVbo = bufferMgr->allocate(barVerts.size() * sizeof(GLfloat), GL_MAP_WRITE_BIT);
            bufferMgr->update(m_barVbo, barVerts);

            VertexLayout layout;
            layout.indexBuffer = m_sliderIbo;
            layout.vertexBuffers.emplace_back(0, BufferDescriptor{ m_sliderVbo, 0, 2 * sizeof(GLfloat) });
            layout.attributes.emplace_back(0, AttributeFormat{ VertexAttrib::Position, 2, 0 });
            m_sliderVao = renderer->getVertexAssembly()->createInputState(layout);

            VertexLayout barLayout;
            barLayout.indexBuffer = 0;
            barLayout.vertexBuffers.emplace_back(0, BufferDescriptor{ m_barVbo, 0, 2 * sizeof(GLfloat) });
            barLayout.attributes.emplace_back(0, AttributeFormat{ VertexAttrib::Position, 2, 0 });
            m_barVao = renderer->getVertexAssembly()->createInputState(barLayout);

            m_technique->setUniformAttribute("color", m_color);

            m_valueText = std::make_shared<Text>(font, renderer);
            m_valueText->setText(to_string(m_currValue));
            m_valueText->setPosition({ BarWidth + 10.f, m_valueText->getTextSize().y / 2, 0.f });
            m_valueText->setColor({ 0.f, 1.f, 1.f, 1.f });
            addComponent(m_valueText);
        }

        Slider::~Slider()
        {
        }

        void Slider::handleEvents(const Event& event)
        {
            if (event.type == Event::MouseButtonPressed)
            {
                int x = event.mouseButton.x;
                int y = event.mouseButton.y;
                if (isPointInRect(x, y, glm::vec2(m_sliderPos) - m_originOffset, m_sliderSize))
                    m_isDragged = true;
            }
            else if (event.type == Event::MouseButtonReleased)
            {
                m_isDragged = false;
            }
            else if (event.type == Event::MouseMoved)
            {
                int x = event.mouseMove.x;
                int y = event.mouseMove.y;

                auto absPos = getAbsolutePosition();
                if (x < absPos.x || x > absPos.x + BarWidth)
                {
                    x = x < absPos.x ? static_cast<int>(absPos.x) :
                        x > absPos.x + BarWidth ?
                        static_cast<int>(absPos.x + BarWidth) : x;
                    m_isDragged = false;
                }

                if (y < absPos.y - SliderHeight / 2 || y > absPos.y + SliderHeight / 2)
                    m_isDragged = false;

                if (m_isDragged)
                {
                    m_sliderPos.x = static_cast<float>(x);
                    m_sliderMat[3][0] = static_cast<float>(x);

                    float factor = (m_sliderPos.x - absPos.x) / BarWidth;

                    int newValue = static_cast<int>(factor * m_values + 1.5f);

                    if (newValue != m_currValue)
                    {
                        m_currValue = newValue;
                        m_valueText->setText(to_string(m_currValue));

                        if (m_callback)
                            m_callback(m_currValue);
                    }
                }
            }
        }

        void Slider::render(const glm::mat4& P)
        {
            glUseProgram(m_technique->getRawHandle());

            m_technique->setUniformAttribute("MVP", P * m_modelMat);
            glBindVertexArray(m_barVao);
            glDrawArrays(GL_LINES, 0, 2);

            m_technique->setUniformAttribute("MVP", P * m_sliderMat);
            glBindVertexArray(m_sliderVao);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

            m_valueText->render(P);
        }

        void Slider::setPosition(const glm::vec3& position)
        {
            Component::setPosition(position);
            updateSliderPosition();
        }

        glm::vec4 Slider::getBounds() const
        {
            glm::vec3 absPos = getAbsolutePosition();
            glm::vec2 clickSize(BarWidth, 10.f);
            return glm::vec4(absPos.x - 3.f, absPos.y - clickSize.y / 2 - 3.f, absPos.x + clickSize.x + 3.f, absPos.y + clickSize.y / 2 + 3.f);
        }

        int Slider::getValue() const
        {
            return m_currValue;
        }

        void Slider::setCallback(std::function<void(int)> callback)
        {
            m_callback = callback;
        }

        void Slider::updateModelMatrix()
        {
            Component::updateModelMatrix();
            updateSliderPosition();
        }

        void Slider::updateSliderPosition()
        {
            m_sliderPos = getAbsolutePosition();
            auto xIncr = BarWidth / m_values * m_currValue;
            m_sliderPos.x += xIncr;

            m_sliderMat = m_modelMat;
            m_sliderMat[3][0] += xIncr;
        }

        std::vector<GLfloat> Slider::generateSliderVertices(float width, float height)
        {
            std::vector<GLfloat> verts;
            
            verts.push_back(-width / 2);
            verts.push_back(-height / 2);

            verts.push_back(width / 2);
            verts.push_back(-height / 2);

            verts.push_back(width / 2);
            verts.push_back(height / 2);

            verts.push_back(-width / 2);
            verts.push_back(height / 2);
            
            return verts;
        }

        std::vector<GLushort> Slider::generateSliderIndices()
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

        std::vector<GLfloat> Slider::generateBarVertices()
        {
            std::vector<GLfloat> barVertices;

            barVertices.push_back(0.f);
            barVertices.push_back(0.f);
            barVertices.push_back(BarWidth);
            barVertices.push_back(0.f);

            return barVertices;
        }
    }
}