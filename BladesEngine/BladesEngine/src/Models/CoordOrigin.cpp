#include "Models/CoordOrigin.h"

#include "Renderer/GLRenderer.h"
#include "Renderer/Technique.h"

namespace fsi
{
    CoordOrigin::CoordOrigin(float size, GLRenderer* renderer)
    {
        auto vertexData = generateVertices(size);

        auto bufferMgr = renderer->getDeviceBufferManager();
        GLuint vbo = bufferMgr->allocate(vertexData.size() * sizeof(GLfloat), GL_MAP_WRITE_BIT);
        bufferMgr->update(vbo, vertexData);

        VertexLayout layout;
        layout.indexBuffer = 0;
        layout.vertexBuffers.emplace_back(0, BufferDescriptor{ vbo, 0, 7 * sizeof(GLfloat) });
        layout.attributes.emplace_back(0, AttributeFormat{ VertexAttrib::Position, 3, 0 });
        layout.attributes.emplace_back(0, AttributeFormat{ VertexAttrib::Color   , 4, 12 });
        auto vao = renderer->getVertexAssembly()->createInputState(layout);

        auto techId = renderer->getTechniqueCache()->getProgram("color");
        m_technique = std::make_unique<Technique>(techId);

        m_drawItem.program = techId;
        m_drawItem.primitiveType = GL_LINES;
        m_drawItem.vertexArray = vao;
        m_drawItem.numVerts = vertexData.size() / 7;
        m_drawItem.numIndices = 0;
        m_drawItem.baseVertex = 0;
        m_drawItem.updateUniforms = [this](const glm::mat4& P, const glm::mat4& V)
        {
            m_technique->use();
            glm::mat4 lModel(1.f);
            glm::mat4 lineMat = P * V * lModel;

            m_technique->setUniformAttribute("MVP", lineMat);
        };

        renderer->submitDrawItem(m_drawItem);
    }

    CoordOrigin::~CoordOrigin()
    {

    }

    std::vector<GLfloat> CoordOrigin::generateVertices(float size)
    {
        std::vector<GLfloat> vertices;

        // X axis
        vertices.push_back(0.f);
        vertices.push_back(0.f);
        vertices.push_back(0.f);
        vertices.push_back(1.f);
        vertices.push_back(0.f);
        vertices.push_back(0.f);
        vertices.push_back(1.f);
        vertices.push_back(size);
        vertices.push_back(0.f);
        vertices.push_back(0.f);
        vertices.push_back(1.f);
        vertices.push_back(0.f);
        vertices.push_back(0.f);
        vertices.push_back(1.f);

        // Y axis
        vertices.push_back(0.f);
        vertices.push_back(0.f);
        vertices.push_back(0.f);
        vertices.push_back(0.f);
        vertices.push_back(1.f);
        vertices.push_back(0.f);
        vertices.push_back(1.f);
        vertices.push_back(0.f);
        vertices.push_back(size);
        vertices.push_back(0.f);
        vertices.push_back(0.f);
        vertices.push_back(1.f);
        vertices.push_back(0.f);
        vertices.push_back(1.f);

        // Z axis
        vertices.push_back(0.f);
        vertices.push_back(0.f);
        vertices.push_back(0.f);
        vertices.push_back(0.f);
        vertices.push_back(0.f);
        vertices.push_back(1.f);
        vertices.push_back(1.f);
        vertices.push_back(0.f);
        vertices.push_back(0.f);
        vertices.push_back(size);
        vertices.push_back(0.f);
        vertices.push_back(0.f);
        vertices.push_back(1.f);
        vertices.push_back(1.f);

        return vertices;
    }
}