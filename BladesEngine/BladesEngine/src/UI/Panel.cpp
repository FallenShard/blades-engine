#include <vector>
#include "UI/Panel.h"
#include "Renderer/Technique.h"
#include "Renderer/VertexAssembly.h"

namespace fsi
{
    Panel::Panel(glm::vec3&& position, glm::vec2&& size)
        : m_position(position)
        , m_size(size)
        , m_color(glm::vec4(0.0f, 0.0f, 0.0f, 0.8f))
        , m_borderColor(glm::vec4(0.0f, 1.0f, 1.0f, 1.f))
        , m_vao(0)
        , m_modelMat(glm::translate(position) * glm::scale(glm::vec3(size.x, size.y, 1)))
        , m_ibo(0)
        , m_vbo(0)
    {
        std::vector<GLfloat> panelVerts;
        std::vector<GLushort> panelIndices;
        std::vector<GLushort> borderIndices;
        panelVerts.push_back(0.0f);
        panelVerts.push_back(0.0f);

        panelVerts.push_back(1.f);
        panelVerts.push_back(0.f);
    
        panelVerts.push_back(1.f);
        panelVerts.push_back(1.f);

        panelVerts.push_back(0.f);
        panelVerts.push_back(1.f);

        panelIndices.push_back(0);
        panelIndices.push_back(2);
        panelIndices.push_back(1);

        panelIndices.push_back(0);
        panelIndices.push_back(3);
        panelIndices.push_back(2);

        borderIndices.push_back(0);
        borderIndices.push_back(3);
        borderIndices.push_back(2);
        borderIndices.push_back(1);

        glCreateVertexArrays(1, &m_vao);
        glCreateVertexArrays(1, &m_borderVao);
        glCreateBuffers(1, &m_vbo);
        glCreateBuffers(1, &m_ibo);
        glCreateBuffers(1, &m_borderIbo);

        glNamedBufferData(m_vbo, panelVerts.size() * sizeof(GLfloat), panelVerts.data(), GL_STATIC_DRAW);
        glNamedBufferData(m_ibo, panelIndices.size() * sizeof(GLushort), panelIndices.data(), GL_STATIC_DRAW);
        glNamedBufferData(m_borderIbo, borderIndices.size() * sizeof(GLushort), borderIndices.data(), GL_STATIC_DRAW);

        glVertexArrayVertexBuffer(m_vao, VertexBufferBinding::Slot0, m_vbo, 0, 2 * sizeof(GLfloat));
        glVertexArrayAttribBinding(m_vao, VertexAttrib::Position, VertexBufferBinding::Slot0);
        glVertexArrayAttribFormat(m_vao, VertexAttrib::Position, 2, GL_FLOAT, GL_FALSE, 0);
        glEnableVertexArrayAttrib(m_vao, VertexAttrib::Position);
        glVertexArrayElementBuffer(m_vao, m_ibo);

        glVertexArrayVertexBuffer(m_borderVao, VertexBufferBinding::Slot0, m_vbo, 0, 2 * sizeof(GLfloat));
        glVertexArrayAttribBinding(m_borderVao, VertexAttrib::Position, VertexBufferBinding::Slot0);
        glVertexArrayAttribFormat(m_borderVao, VertexAttrib::Position, 2, GL_FLOAT, GL_FALSE, 0);
        glEnableVertexArrayAttrib(m_borderVao, VertexAttrib::Position);
        glVertexArrayElementBuffer(m_borderVao, m_borderIbo);
    }

    Panel::~Panel()
    {
    }

    void Panel::setPosition(glm::vec3&& position)
    {
        m_position = position;
        m_modelMat = glm::translate(position);
    }

    glm::vec3 Panel::getPosition() const
    {
        return m_position;
    }

    void Panel::setSize(glm::vec2&& size)
    {
        m_size = size;
    }

    void Panel::setColor(glm::vec4&& color)
    {
        m_color = color;
    }

    void Panel::render(Technique* program, const glm::mat4& parent)
    {
        program->setUniformAttribute("color", m_color);
        program->setUniformAttribute("MVP", parent * m_modelMat);

        glBindVertexArray(m_vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

        program->setUniformAttribute("color", m_borderColor);
        glBindVertexArray(m_borderVao);
        glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, nullptr);
    }
}