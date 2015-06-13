#include "OglWrapper/VertexStreams.h"
#include "Utils/Font.h"
#include "UI/Text.h"

namespace fsi
{

using Glyph = Font::GlyphInfo;

Text::Text(std::shared_ptr<Font>& textRenderer)
    : m_textRenderer(textRenderer)
    , m_textString("")
    , m_atlasTexId(textRenderer->getAtlasTexId())
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
    m_vertices.clear(); 
    m_indices.clear();

    int ind = 0;
    float currentX = 0.f;
    float currentY = 0.f;
    float atlasWidth = static_cast<float>(m_textRenderer->getAtlasWidth());
    float atlasHeight = static_cast<float>(m_textRenderer->getAtlasHeight());

    for (auto& character : newText)
    {
        const Glyph& gInfo = m_textRenderer->getGlyphInfo(character);
        float x1 = currentX + gInfo.bmpLeft;
        float y1 = currentY - gInfo.bmpTop;
        float x2 = x1 + gInfo.bmpWidth;
        float y2 = y1 + gInfo.bmpHeight;

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

void Text::setPosition(glm::vec2 position)
{

}

void Text::render()
{
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_SHORT, nullptr);
}

}