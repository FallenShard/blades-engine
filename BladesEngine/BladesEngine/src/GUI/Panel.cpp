#include <vector>

#include "GUI/Panel.h"

#include "Renderer/GLRenderer.h"
#include "Renderer/Technique.h"
#include "Renderer/VertexAssembly.h"

namespace fsi
{
    namespace gui
    {
        Panel::Panel(GLRenderer* renderer, const glm::vec3& position, const glm::vec2& size)
            : Component(position, size)
            , m_color(glm::vec4(0.0f, 0.0f, 0.0f, 0.8f))
            , m_borderColor(glm::vec4(0.0f, 1.0f, 1.0f, 1.f))
            , m_vao(0)
            , m_ibo(0)
            , m_vbo(0)
            , m_borderVao(0)
            , m_borderIbo(0)
        {
            std::vector<GLfloat> panelVerts = generateQuadVertices();
            std::vector<GLushort> panelIndices = generateQuadIndices();
            std::vector<GLushort> borderIndices = generateQuadBorderIndices();

            auto bufferMgr = renderer->getDeviceBufferManager();
            m_vbo = bufferMgr->allocate(panelVerts.size() * sizeof(GLfloat), GL_MAP_WRITE_BIT);
            bufferMgr->update(m_vbo, panelVerts);
            m_ibo = bufferMgr->allocate(panelIndices.size() * sizeof(GLushort), GL_MAP_WRITE_BIT);
            bufferMgr->update(m_ibo, panelIndices);
            m_borderIbo = bufferMgr->allocate(borderIndices.size() * sizeof(GLushort), GL_MAP_WRITE_BIT);
            bufferMgr->update(m_borderIbo, borderIndices);

            VertexLayout layout;
            layout.indexBuffer = m_ibo;
            layout.vertexBuffers.emplace_back(0, BufferDescriptor{ m_vbo, 0, 2 * sizeof(GLfloat) });
            layout.attributes.emplace_back(0, AttributeFormat{ VertexAttrib::Position, 2, 0 });
            m_vao = renderer->getVertexAssembly()->createInputState(layout);

            layout.indexBuffer = m_borderIbo;
            m_borderVao = renderer->getVertexAssembly()->createInputState(layout);

            m_technique = std::make_unique<Technique>(renderer->getTechniqueCache()->getProgram("simple"));
        }

        Panel::~Panel()
        {
        }

        void Panel::setColor(const glm::vec4& color)
        {
            m_color = color;
        }

        void Panel::setBorderColor(const glm::vec4 & color)
        {
            m_borderColor = color;
        }

        void Panel::render(const glm::mat4& P)
        {
            glUseProgram(m_technique->getRawHandle());
            m_technique->setUniformAttribute("MVP", P * m_modelMat);

            m_technique->setUniformAttribute("color", m_color);
            glBindVertexArray(m_vao);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

            m_technique->setUniformAttribute("color", m_borderColor);
            glBindVertexArray(m_borderVao);
            glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, nullptr);

            for (auto& child : m_children)
                child->render(P);
        }

        std::vector<GLfloat> Panel::generateQuadVertices()
        {
            std::vector<GLfloat> vertexData;

            vertexData.push_back(0.0f);
            vertexData.push_back(0.0f);

            vertexData.push_back(1.f);
            vertexData.push_back(0.f);

            vertexData.push_back(1.f);
            vertexData.push_back(1.f);

            vertexData.push_back(0.f);
            vertexData.push_back(1.f);

            return vertexData;
        }

        std::vector<GLushort> Panel::generateQuadIndices()
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

        std::vector<GLushort> Panel::generateQuadBorderIndices()
        {
            std::vector<GLushort> indices;

            indices.push_back(0);
            indices.push_back(3);
            indices.push_back(2);
            indices.push_back(1);

            return indices;
        }
    }
}