#include <algorithm>

#include "Utils/Font.h"
#include "Renderer/GLRenderer.h"
#include "Renderer/Technique.h"

#include "GUI/Text.h"

namespace fsi
{
    namespace gui
    {
        using Glyph = Font::GlyphInfo;

        Text::Text(std::shared_ptr<Font>& font, GLRenderer* renderer)
            : m_font(font)
            , m_renderer(renderer)
            , m_textString("")
            , m_textSize(glm::vec2(0.f))
            , m_vao(0)
            , m_vbo(0)
            , m_ibo(0)
        {
            auto bufferMgr = renderer->getDeviceBufferManager();
            m_vbo = bufferMgr->allocateMutable(0, GL_DYNAMIC_DRAW);
            m_ibo = bufferMgr->allocateMutable(0, GL_DYNAMIC_DRAW);

            VertexLayout layout;
            layout.indexBuffer = m_ibo;
            layout.vertexBuffers.emplace_back(0, BufferDescriptor{ m_vbo, 0, 4 * sizeof(GLfloat) });
            layout.attributes.emplace_back(0, AttributeFormat{ VertexAttrib::Position, 4, 0 });
            m_vao = renderer->getVertexAssembly()->createInputState(layout);
            
            m_technique = std::make_unique<Technique>(renderer->getTechniqueCache()->getProgram("text"));
        }

        Text::~Text()
        {
        }

        void Text::setText(std::string newText)
        {
            m_textString = newText;
            m_textSize = glm::vec2(0.f, 0.f);
            m_vertices.clear();
            m_indices.clear();

            int ind = 0;
            float currentX = 0.f;
            float currentY = 0.f;
            float atlasWidth = static_cast<float>(m_font->getAtlasWidth());
            float atlasHeight = static_cast<float>(m_font->getAtlasHeight());

            for (auto& character : newText)
            {
                auto gInfo = m_font->getGlyphInfo(character);
                float x1 = currentX + gInfo.bmpLeft;
                float y1 = currentY - gInfo.bmpTop;
                float x2 = x1 + gInfo.bmpWidth;
                float y2 = y1 + gInfo.bmpHeight;
                m_textSize.y = std::max(m_textSize.y, gInfo.bmpHeight);
                m_textSize.x = x2;

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

            m_renderer->getDeviceBufferManager()->updateMutable(m_vbo, m_vertices);
            m_renderer->getDeviceBufferManager()->updateMutable(m_ibo, m_indices);
        }

        void Text::setColor(const glm::vec4& color)
        {
            m_color = color;
        }

        void Text::render(const glm::mat4& P)
        {
            glUseProgram(m_technique->getRawHandle());
            m_technique->setUniformAttribute("color", m_color);
            m_technique->setUniformAttribute("MVP", P * m_modelMat);

            glBindVertexArray(m_vao);
            glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_SHORT, nullptr);
        }
    }
}