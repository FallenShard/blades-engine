#include "UI/Slider.h"
#include "Renderer/VertexAssembly.h"
#include "Renderer/Technique.h"
#include "Input/Event.h"
#include <iostream>

namespace fsi
{
    namespace
    {
        float m_sliderWidth = 10.f;
        float m_sliderHeight = 15.f;

        glm::vec2 m_originOffset(m_sliderWidth / 2, m_sliderHeight / 2);

        float m_barWidth = 100.f;

        bool isPointInRect(int x, int y, glm::vec2& base, glm::vec2& size)
        {
            if (x < base.x || x >(base.x + size.x) || y < base.y || y >(base.y + size.y))
                return false;
            return true;
        }

        int values = 20;
    }

    Slider::Slider()
        : m_value(0)
    {
        std::vector<GLfloat> sliderVerts;
        std::vector<GLushort> sliderIndices;
        sliderVerts.push_back(-m_sliderWidth / 2);
        sliderVerts.push_back(-m_sliderHeight / 2);

        sliderVerts.push_back(m_sliderWidth / 2);
        sliderVerts.push_back(-m_sliderHeight / 2);

        sliderVerts.push_back(m_sliderWidth / 2);
        sliderVerts.push_back(m_sliderHeight / 2);

        sliderVerts.push_back(-m_sliderWidth / 2);
        sliderVerts.push_back(m_sliderHeight / 2);

        sliderIndices.push_back(0);
        sliderIndices.push_back(2);
        sliderIndices.push_back(1);

        sliderIndices.push_back(0);
        sliderIndices.push_back(3);
        sliderIndices.push_back(2);

        std::vector<GLfloat> barVerts;
        barVerts.push_back(0.f);
        barVerts.push_back(0.f);
        barVerts.push_back(m_barWidth);
        barVerts.push_back(0.f);

        glCreateVertexArrays(1, &m_sliderVao);
        glCreateBuffers(1, &m_sliderVbo);
        glCreateBuffers(1, &m_sliderIbo);

        glNamedBufferData(m_sliderVbo, sliderVerts.size() * sizeof(GLfloat), sliderVerts.data(), GL_STATIC_DRAW);
        glNamedBufferData(m_sliderIbo, sliderIndices.size() * sizeof(GLushort), sliderIndices.data(), GL_STATIC_DRAW);

        glVertexArrayVertexBuffer(m_sliderVao, VertexBufferBinding::Slot0, m_sliderVbo, 0, 2 * sizeof(GLfloat));
        glVertexArrayAttribBinding(m_sliderVao, VertexAttrib::Position, VertexBufferBinding::Slot0);
        glVertexArrayAttribFormat(m_sliderVao, VertexAttrib::Position, 2, GL_FLOAT, GL_FALSE, 0);
        glEnableVertexArrayAttrib(m_sliderVao, VertexAttrib::Position);
        glVertexArrayElementBuffer(m_sliderVao, m_sliderIbo);

        glCreateVertexArrays(1, &m_barVao);
        glCreateBuffers(1, &m_barVbo);

        glNamedBufferData(m_barVbo, barVerts.size() * sizeof(GLfloat), barVerts.data(), GL_STATIC_DRAW);

        glVertexArrayVertexBuffer(m_barVao, VertexBufferBinding::Slot0, m_barVbo, 0, 2 * sizeof(GLfloat));
        glVertexArrayAttribBinding(m_barVao, VertexAttrib::Position, VertexBufferBinding::Slot0);
        glVertexArrayAttribFormat(m_barVao, VertexAttrib::Position, 2, GL_FLOAT, GL_FALSE, 0);
        glEnableVertexArrayAttrib(m_barVao, VertexAttrib::Position);

        m_modelMat = glm::translate(glm::vec3(0.f, 0.f, 0.f));
        m_size = glm::vec2(100.f, 0.f);
        m_sliderSize = glm::vec2(m_sliderWidth, m_sliderHeight);

        m_isDragged = false;
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
            if (isPointInRect(x, y, m_sliderPos - m_originOffset, m_sliderSize))
            {
                m_isDragged = true;
            }
        }
        else if (event.type == Event::MouseButtonReleased)
        {
            m_isDragged = false;
        }
        else if (event.type == Event::MouseMoved)
        {
            int x = event.mouseMove.x;
            int y = event.mouseMove.y;

            if (x < m_position.x || x > m_position.x + m_barWidth)
            {
                x = x < m_position.x ? static_cast<int>(m_position.x) :
                    x > m_position.x + m_barWidth ?
                    static_cast<int>(m_position.x + m_barWidth) : x;
            }
            

            if (m_isDragged)
            {
                m_sliderPos.x = static_cast<float>(x);
                m_sliderMat[3][0] = static_cast<float>(x - 50.f);

                float factor = (m_sliderPos.x - m_position.x) / m_barWidth;
            
                int newValue = static_cast<int>(factor * values + 1.5f);

                if (newValue != m_value)
                {
                    m_value = newValue;

                    if (m_callback)
                        m_callback(m_value);
                }
           
            }
        }
    }

    void Slider::render(Technique* prog, Technique* textProg, const glm::mat4& parent)
    {
        prog->use();
        prog->setUniformAttribute("MVP", parent * m_sliderMat);

        glBindVertexArray(m_sliderVao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

        prog->setUniformAttribute("MVP", parent * m_modelMat);
        glBindVertexArray(m_barVao);
        glDrawArrays(GL_LINES, 0, 2);
    }

    void Slider::setPosition(glm::vec3& position)
    {
        m_position = glm::vec2(position);
        m_position += glm::vec2(50.f, 50.f);

        m_sliderPos = glm::vec2(position);
        m_sliderPos += glm::vec2(50.f, 50.f);
        m_sliderPos.x += m_barWidth / values * 8;

        m_modelMat = glm::translate(position);
        m_sliderMat = glm::translate(position);
        m_sliderMat[3][0] += m_barWidth / values * 8;
    }

    void Slider::setCallback(std::function<void(int)> callback)
    {
        m_callback = callback;
    }
}