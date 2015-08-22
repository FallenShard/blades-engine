#include "UI/CheckBox.h"
#include "UI/Text.h"
#include "Renderer/VertexAssembly.h"
#include "Renderer/Technique.h"
#include "Input/Event.h"

#include <iostream>

namespace fsi
{
    namespace
    {
        GLfloat CheckBoxSize = 24.f;

        bool isPointInRect(int x, int y, glm::vec2& base, glm::vec2& size)
        {
            if (x < base.x || x >(base.x + size.x) || y < base.y || y >(base.y + size.y))
                return false;
            return true;
        }
    }

    CheckBox::CheckBox(bool isChecked)
        : m_vao(0)
        , m_vbo(0)
        , m_ibo(0)
        , m_isChecked(isChecked)
        , m_size(glm::vec2(CheckBoxSize, CheckBoxSize))
        , m_mouseAreaSize(glm::vec2(CheckBoxSize, CheckBoxSize))
    {
        glCreateVertexArrays(1, &m_vao);
        glCreateBuffers(1, &m_vbo);
        glCreateBuffers(1, &m_ibo);

        setState(m_isChecked);

        glVertexArrayVertexBuffer(m_vao, VertexBufferBinding::Slot0, m_vbo, 0, 4 * sizeof(GLfloat));
        glVertexArrayElementBuffer(m_vao, m_ibo);

        glVertexArrayAttribBinding(m_vao, VertexAttrib::Position, VertexBufferBinding::Slot0);
        glVertexArrayAttribFormat(m_vao, VertexAttrib::Position, 4, GL_FLOAT, GL_FALSE, 0);
        glEnableVertexArrayAttrib(m_vao, VertexAttrib::Position);
    }

    CheckBox::~CheckBox()
    {
        glDeleteVertexArrays(1, &m_vao);
        glDeleteBuffers(1, &m_vbo);
        glDeleteBuffers(1, &m_ibo);
    }

    void CheckBox::setState(bool isChecked)
    {
        m_vertices.clear();
        m_indices.clear();

        float texOffset = isChecked ? 0.f : 0.5f;

        m_vertices.push_back(0.f);
        m_vertices.push_back(0.f);
        m_vertices.push_back(0.f + texOffset);
        m_vertices.push_back(0.f);

        m_vertices.push_back(CheckBoxSize);
        m_vertices.push_back(0.f);
        m_vertices.push_back(0.49f + texOffset);
        m_vertices.push_back(0.0f);

        m_vertices.push_back(CheckBoxSize);
        m_vertices.push_back(CheckBoxSize);
        m_vertices.push_back(0.49f + texOffset);
        m_vertices.push_back(1.f);

        m_vertices.push_back(0.f);
        m_vertices.push_back(CheckBoxSize);
        m_vertices.push_back(0.f + texOffset);
        m_vertices.push_back(1.f);

        m_indices.push_back(0);
        m_indices.push_back(2);
        m_indices.push_back(1);

        m_indices.push_back(0);
        m_indices.push_back(3);
        m_indices.push_back(2);

        glNamedBufferData(m_vbo, m_vertices.size() * sizeof(GLfloat), m_vertices.data(), GL_DYNAMIC_DRAW);
        glNamedBufferData(m_ibo, m_indices.size() * sizeof(GLushort), m_indices.data(), GL_DYNAMIC_DRAW);
    }

    void CheckBox::render(Technique* program, const glm::mat4& parentMat)
    {
        program->setUniformAttribute("MVP", parentMat * m_modelMat);

        glBindVertexArray(m_vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
    }

    void CheckBox::setPosition(glm::vec3& position)
    {
        m_position = glm::vec2(position);
        m_position += glm::vec2(50.f, 50.f);
        m_modelMat = glm::translate(position);
    }

    void CheckBox::addMouseArea(float xIncrease, float yIncrease)
    {
        m_mouseAreaSize.x += xIncrease;
        m_mouseAreaSize.y += yIncrease;
    }

    void CheckBox::setCallback(std::function<void(bool)> callback)
    {
        m_callback = callback;
    }

    bool CheckBox::handleEvents(const Event& event)
    {
        if (event.type == Event::MouseButtonReleased)
        {
            int x = event.mouseButton.x;
            int y = event.mouseButton.y;

            if (isPointInRect(x, y, m_position, m_mouseAreaSize))
            {
                m_isChecked = !m_isChecked;
                setState(m_isChecked);

                if (m_callback)
                    m_callback(m_isChecked);

                return true;
            }
        }

        return false;
    }

    glm::vec2 CheckBox::getSize() const
    {
        return m_size;
    }
}