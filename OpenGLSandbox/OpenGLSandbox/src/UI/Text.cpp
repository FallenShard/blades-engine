#include "OglWrapper/VertexStreams.h"
#include "Utils/Font.h"
#include "UI/Text.h"
#include <algorithm>

namespace fsi
{

using Glyph = Font::GlyphInfo;

Text::Text(std::shared_ptr<Font>& font)
    : m_font(font)
    , m_textString("")
    , m_atlasTexId(font->getAtlasTexId())
    , m_vao(0)
    , m_vbo(0)
    , m_ibo(0)
{
    glCreateVertexArrays(1, &m_vao);
    glCreateBuffers(1, &m_vbo);
    glCreateBuffers(1, &m_ibo);

    glVertexArrayVertexBuffer(m_vao, VertexBufferBinding::Slot0, m_vbo, 0, 4 * sizeof(GLfloat));
    glVertexArrayElementBuffer(m_vao, m_ibo);

    glVertexArrayAttribBinding(m_vao, VertexAttrib::Position, VertexBufferBinding::Slot0);
    glVertexArrayAttribFormat(m_vao, VertexAttrib::Position, 4, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(m_vao, VertexAttrib::Position);
}

Text::~Text()
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ibo);
}

void Text::setText(std::string newText)
{
    m_textString = newText;
    m_size = glm::vec2(0.f, 0.f);
    m_vertices.clear(); 
    m_indices.clear();

    int ind = 0;
    float currentX = 0.f;
    float currentY = 0.f;
    float atlasWidth = static_cast<float>(m_font->getAtlasWidth());
    float atlasHeight = static_cast<float>(m_font->getAtlasHeight());

    for (auto& character : newText)
    {
        const Glyph& gInfo = m_font->getGlyphInfo(character);
        float x1 = currentX + gInfo.bmpLeft;
        float y1 = currentY - gInfo.bmpTop;
        float x2 = x1 + gInfo.bmpWidth;
        float y2 = y1 + gInfo.bmpHeight;
        m_size.y = std::max(m_size.x, gInfo.bmpHeight);
        m_size.x = x2;

        // Advance the cursor to the start of the next character
        currentX += gInfo.advanceX;
        currentY += gInfo.advanceY;

        m_vertices.push_back(x1);
        m_vertices.push_back(y1);
        m_vertices.push_back(gInfo.atlasOffsetX);
        m_vertices.push_back(0.0f);

        m_vertices.push_back(x2);
        m_vertices.push_back(y1);
        m_vertices.push_back(gInfo.atlasOffsetX + gInfo.bmpWidth / atlasWidth);
        m_vertices.push_back(0.0f);

        m_vertices.push_back(x2);
        m_vertices.push_back(y2);
        m_vertices.push_back(gInfo.atlasOffsetX + gInfo.bmpWidth / atlasWidth);
        m_vertices.push_back(gInfo.bmpHeight / atlasHeight);

        m_vertices.push_back(x1);
        m_vertices.push_back(y2);
        m_vertices.push_back(gInfo.atlasOffsetX);
        m_vertices.push_back(gInfo.bmpHeight / atlasHeight);

        m_indices.push_back(ind + 0);
        m_indices.push_back(ind + 2);
        m_indices.push_back(ind + 1);

        m_indices.push_back(ind + 0);
        m_indices.push_back(ind + 3);
        m_indices.push_back(ind + 2);

        ind += 4;
    }

    glNamedBufferData(m_vbo, m_vertices.size() * sizeof(GLfloat), m_vertices.data(), GL_DYNAMIC_DRAW);
    glNamedBufferData(m_ibo, m_indices.size() * sizeof(GLushort), m_indices.data(), GL_DYNAMIC_DRAW);
}

void Text::setPosition(glm::vec3&& position)
{
    m_modelMat = glm::translate(position);
}

void Text::setColor(glm::vec4&& color)
{
    m_color = color;
}

void Text::render(ShaderProgram* program, const glm::mat4& parent)
{
    program->setUniformAttribute("color", m_color);
    program->setUniformAttribute("MVP", parent * m_modelMat);

    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_SHORT, nullptr);
}

glm::vec2 Text::getTextSize() const
{
    return m_size;
}

}