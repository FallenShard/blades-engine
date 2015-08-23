#include <vector>

#include "Renderer/GLRenderer.h"
#include "Renderer/Technique.h"

#include "Models/Skybox.h"

namespace fsi
{
    Skybox::Skybox(float size, GLRenderer* renderer)
        : m_technique(std::make_unique<Technique>(renderer->getTechniqueCache()->getProgram("skybox")))
    {
        std::vector<GLfloat> vertexData = generateVertices(size);
        std::vector<GLushort> indexData = generateIndices();

        auto bufferMgr = renderer->getDeviceBufferManager();
        auto vbo = bufferMgr->allocate(vertexData.size() * sizeof(GLfloat), GL_MAP_WRITE_BIT);
        bufferMgr->update(vbo, vertexData);
        auto ibo = bufferMgr->allocate(indexData.size() * sizeof(GLushort), GL_MAP_WRITE_BIT);
        bufferMgr->update(ibo, indexData);

        VertexLayout layout;
        layout.indexBuffer = ibo;
        layout.vertexBuffers.emplace_back(0, BufferDescriptor{ vbo, 0, 6 * sizeof(GLfloat) });
        layout.attributes.emplace_back(0, AttributeFormat{ VertexAttrib::Position, 3, 0 });
        layout.attributes.emplace_back(0, AttributeFormat{ VertexAttrib::TexCoord0, 3, 3 * sizeof(GLfloat) });
        auto vao = renderer->getVertexAssembly()->createInputState(layout);

        std::vector<std::string> fileNames;
        fileNames.emplace_back("space_left.png");
        fileNames.emplace_back("space_front.png");
        fileNames.emplace_back("space_right.png");
        fileNames.emplace_back("space_back.png");
        fileNames.emplace_back("space_top.png");
        fileNames.emplace_back("space_bottom.png");
        auto texArray = renderer->getTextureManager()->load3DTexture(fileNames, 1, InternalFormat::RGBA8, BaseFormat::RGBA);
        auto sampler = renderer->getTextureManager()->getSamplerPreset(TextureManager::LinearClamp);
        auto skyboxTexInfo = renderer->getTextureManager()->createTextureInfo(texArray, sampler);

        m_technique->setUniformAttribute("skybox", skyboxTexInfo.unit);

        m_modelMatrix = glm::mat4(1.f);

        m_drawItem.program = renderer->getTechniqueCache()->getProgram("skybox");
        m_drawItem.numVerts = vertexData.size() / 6;
        m_drawItem.numIndices = indexData.size();
        m_drawItem.baseVertex = 0;
        m_drawItem.primitiveType = GL_TRIANGLES;
        m_drawItem.vertexArray = vao;
        m_drawItem.preRender = [this](const glm::mat4& P, const glm::mat4& V)
        {
            m_technique->setUniformAttribute("MVP", P * (V * m_modelMatrix));

            glDisable(GL_DEPTH_TEST);
        };
        m_drawItem.postRender = []()
        {
            glEnable(GL_DEPTH_TEST);
        };

        renderer->submitDrawItem(m_drawItem);
    }

    Skybox::~Skybox()
    {

    }

    void Skybox::update(const glm::vec3& cameraPos)
    {
        m_modelMatrix = glm::translate(cameraPos);
    }

    std::vector<GLfloat> Skybox::generateVertices(float size)
    {
        std::vector<GLfloat> vertexData;
        // Front face
        vertexData.push_back(-size);
        vertexData.push_back(-size);
        vertexData.push_back(-size);
        vertexData.push_back(0.f);
        vertexData.push_back(1.f);
        vertexData.push_back(1.f);

        vertexData.push_back(+size);
        vertexData.push_back(-size);
        vertexData.push_back(-size);
        vertexData.push_back(1.f);
        vertexData.push_back(1.f);
        vertexData.push_back(1.f);

        vertexData.push_back(+size);
        vertexData.push_back(+size);
        vertexData.push_back(-size);
        vertexData.push_back(1.f);
        vertexData.push_back(0.f);
        vertexData.push_back(1.f);

        vertexData.push_back(-size);
        vertexData.push_back(+size);
        vertexData.push_back(-size);
        vertexData.push_back(0.f);
        vertexData.push_back(0.f);
        vertexData.push_back(1.f);

        // Right face
        vertexData.push_back(+size);
        vertexData.push_back(-size);
        vertexData.push_back(-size);
        vertexData.push_back(0.f);
        vertexData.push_back(1.f);
        vertexData.push_back(2.f);

        vertexData.push_back(+size);
        vertexData.push_back(-size);
        vertexData.push_back(+size);
        vertexData.push_back(1.f);
        vertexData.push_back(1.f);
        vertexData.push_back(2.f);

        vertexData.push_back(+size);
        vertexData.push_back(+size);
        vertexData.push_back(+size);
        vertexData.push_back(1.f);
        vertexData.push_back(0.f);
        vertexData.push_back(2.f);

        vertexData.push_back(+size);
        vertexData.push_back(+size);
        vertexData.push_back(-size);
        vertexData.push_back(0.f);
        vertexData.push_back(0.f);
        vertexData.push_back(2.f);

        // Left face
        vertexData.push_back(-size);
        vertexData.push_back(-size);
        vertexData.push_back(+size);
        vertexData.push_back(0.f);
        vertexData.push_back(1.f);
        vertexData.push_back(0.f);

        vertexData.push_back(-size);
        vertexData.push_back(-size);
        vertexData.push_back(-size);
        vertexData.push_back(1.f);
        vertexData.push_back(1.f);
        vertexData.push_back(0.f);

        vertexData.push_back(-size);
        vertexData.push_back(+size);
        vertexData.push_back(-size);
        vertexData.push_back(1.f);
        vertexData.push_back(0.f);
        vertexData.push_back(0.f);

        vertexData.push_back(-size);
        vertexData.push_back(+size);
        vertexData.push_back(+size);
        vertexData.push_back(0.f);
        vertexData.push_back(0.f);
        vertexData.push_back(0.f);

        // Rear face
        vertexData.push_back(+size);
        vertexData.push_back(-size);
        vertexData.push_back(+size);
        vertexData.push_back(0.f);
        vertexData.push_back(1.f);
        vertexData.push_back(3.f);

        vertexData.push_back(-size);
        vertexData.push_back(-size);
        vertexData.push_back(+size);
        vertexData.push_back(1.f);
        vertexData.push_back(1.f);
        vertexData.push_back(3.f);

        vertexData.push_back(-size);
        vertexData.push_back(+size);
        vertexData.push_back(+size);
        vertexData.push_back(1.f);
        vertexData.push_back(0.f);
        vertexData.push_back(3.f);

        vertexData.push_back(+size);
        vertexData.push_back(+size);
        vertexData.push_back(+size);
        vertexData.push_back(0.f);
        vertexData.push_back(0.f);
        vertexData.push_back(3.f);

        // Bottom face
        vertexData.push_back(-size);
        vertexData.push_back(-size);
        vertexData.push_back(+size);
        vertexData.push_back(0.f);
        vertexData.push_back(1.f);
        vertexData.push_back(5.f);

        vertexData.push_back(+size);
        vertexData.push_back(-size);
        vertexData.push_back(+size);
        vertexData.push_back(1.f);
        vertexData.push_back(1.f);
        vertexData.push_back(5.f);

        vertexData.push_back(+size);
        vertexData.push_back(-size);
        vertexData.push_back(-size);
        vertexData.push_back(1.f);
        vertexData.push_back(0.f);
        vertexData.push_back(5.f);

        vertexData.push_back(-size);
        vertexData.push_back(-size);
        vertexData.push_back(-size);
        vertexData.push_back(0.f);
        vertexData.push_back(0.f);
        vertexData.push_back(5.f);

        // Top face
        vertexData.push_back(-size);
        vertexData.push_back(+size);
        vertexData.push_back(-size);
        vertexData.push_back(0.f);
        vertexData.push_back(1.f);
        vertexData.push_back(4.f);

        vertexData.push_back(+size);
        vertexData.push_back(+size);
        vertexData.push_back(-size);
        vertexData.push_back(1.f);
        vertexData.push_back(1.f);
        vertexData.push_back(4.f);

        vertexData.push_back(+size);
        vertexData.push_back(+size);
        vertexData.push_back(+size);
        vertexData.push_back(1.f);
        vertexData.push_back(0.f);
        vertexData.push_back(4.f);

        vertexData.push_back(-size);
        vertexData.push_back(+size);
        vertexData.push_back(+size);
        vertexData.push_back(0.f);
        vertexData.push_back(0.f);
        vertexData.push_back(4.f);

        return vertexData;
    }

    std::vector<GLushort> Skybox::generateIndices()
    {
        std::vector<GLushort> indices;
        
        // Front face
        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);

        indices.push_back(0);
        indices.push_back(2);
        indices.push_back(3);

        // Right face
        indices.push_back(4);
        indices.push_back(5);
        indices.push_back(6);

        indices.push_back(4);
        indices.push_back(6);
        indices.push_back(7);

        // Left face
        indices.push_back(8);
        indices.push_back(9);
        indices.push_back(10);

        indices.push_back(8);
        indices.push_back(10);
        indices.push_back(11);

        // Rear face
        indices.push_back(12);
        indices.push_back(13);
        indices.push_back(14);

        indices.push_back(12);
        indices.push_back(14);
        indices.push_back(15);

        // Bottom face
        indices.push_back(16);
        indices.push_back(17);
        indices.push_back(18);
        
        indices.push_back(16);
        indices.push_back(18);
        indices.push_back(19);

        // Top face
        indices.push_back(20);
        indices.push_back(21);
        indices.push_back(22);

        indices.push_back(20);
        indices.push_back(22);
        indices.push_back(23);

        return indices;
    }
}