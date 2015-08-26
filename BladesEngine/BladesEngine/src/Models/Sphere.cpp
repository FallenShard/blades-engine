#include <vector>

#include "Renderer/GLRenderer.h"
#include "Renderer/Technique.h"

#include "Models/Sphere.h"

namespace fsi
{
    Sphere::Sphere(float radius, int rings, int slices, GLRenderer* renderer)
        : m_technique(std::make_unique<Technique>(renderer->getTechniqueCache()->getProgram("phong")))
    {
        std::vector<GLfloat> vertexData;
        std::vector<GLushort> indices;
        generateGeometry(vertexData, indices, radius, rings, slices);

        auto bufferMgr = renderer->getDeviceBufferManager();
        auto vbo = bufferMgr->allocate(vertexData.size() * sizeof(GLfloat), GL_MAP_WRITE_BIT);
        bufferMgr->update(vbo, vertexData);
        auto ibo = bufferMgr->allocate(indices.size() * sizeof(GLushort), GL_MAP_WRITE_BIT);
        bufferMgr->update(ibo, indices);

        VertexLayout layout;
        layout.indexBuffer = ibo;
        layout.vertexBuffers.emplace_back(0, BufferDescriptor{ vbo, 0, 3 * sizeof(GLfloat) });
        layout.attributes.emplace_back(0, AttributeFormat{ VertexAttrib::Position, 3, 0 });
        layout.attributes.emplace_back(0, AttributeFormat{ VertexAttrib::Normal, 3, 0 });
        auto vao = renderer->getVertexAssembly()->createInputState(layout);

        Material material;
        material.ambient = glm::vec4(1.f, 0.f, 0.f, 1.f);
        material.diffuse = glm::vec4(1.f, 0.f, 0.f, 1.f);
        material.specular = glm::vec4(1.f, 1.f, 1.f, 1.f);
        material.shininess = 64.f;
        auto ubo = bufferMgr->allocate(sizeof(material), GL_MAP_WRITE_BIT);
        bufferMgr->update(ubo, &material, sizeof(material));

        auto uboDesc = m_technique->createUniformBufferDescriptor("Material", 1, ubo);

        m_modelMatrix = glm::mat4(1.f);

        glLineWidth(3.f);

        m_drawItem.baseVertex = 0;
        m_drawItem.numIndices = indices.size();
        m_drawItem.numVerts = vertexData.size() / 3;
        m_drawItem.primitiveType = GL_TRIANGLES;
        m_drawItem.program = renderer->getTechniqueCache()->getProgram("phong");
        m_drawItem.vertexArray = vao;
        m_drawItem.preRender = [this](const glm::mat4& P, const glm::mat4& V)
        {
            glm::mat4 MV = V * m_modelMatrix;
            m_technique->setUniformAttribute("MV", MV);
            
            glm::mat4 MVP = P * MV;
            m_technique->setUniformAttribute("MVP", MVP);

            glm::mat3 normalMatrix = glm::mat3(glm::inverseTranspose(MV));
            m_technique->setUniformAttribute("normalMatrix", normalMatrix);

            glm::vec4 mvLightPos = V * glm::vec4(2.f, 2.f, 2.f, 1.f);
            m_technique->setUniformAttribute("mvLightPos", mvLightPos);
        };
        m_drawItem.postRender = nullptr;

        renderer->submitDrawItem(m_drawItem);
    }

    Sphere::~Sphere()
    {
    }

    void Sphere::update(const float deltaTime)
    {
    }

    void Sphere::generateGeometry(std::vector<GLfloat>& vertices, std::vector<GLushort>& indices, float radius, int rings, int slices)
    {
        float sliceIncr = 360.f / slices;
        float ringIncr = 180.f / (rings + 1);

        vertices.push_back(0.f);
        vertices.push_back(radius);
        vertices.push_back(0.f);

        // Faces for initial sphere fan cap
        unsigned short vert = 1;
        for (vert = 1; vert < slices; vert++)
        {
            indices.push_back(vert + 1);
            indices.push_back(0);
            indices.push_back(vert);
        }

        indices.push_back(1);
        indices.push_back(0);
        indices.push_back(vert);

        vert = slices + 1;

        // Intermediate vertices and faces
        for (int ring = 1; ring <= rings; ring++)
        {
            float yCos = cos(glm::radians(ring * ringIncr));
            float ySin = sin(glm::radians(ring * ringIncr));

            for (int slice = slices; slice > 0; slice--)
            {
                float rads = glm::radians(slice * sliceIncr);
                float xCos = cos(rads);
                float zSin = sin(rads);
                vertices.push_back(radius * ySin * xCos);
                vertices.push_back(radius * yCos);
                vertices.push_back(radius * ySin * zSin);
            }

            if (ring == rings)
                break;

            short int prevStart = vert - slices;
            short int prevEnd = vert - 1;
            short int curr;
            for (curr = 0; curr < slices - 1; curr++)
            {
                indices.push_back(prevStart + curr);
                indices.push_back(vert);
                indices.push_back(prevStart + curr + 1);

                indices.push_back(vert + 1);
                indices.push_back(prevStart + curr + 1);
                indices.push_back(vert);
                vert++;
            }

            indices.push_back(prevStart);
            indices.push_back(prevEnd);
            indices.push_back(vert);

            indices.push_back(prevEnd + 1);
            indices.push_back(prevStart);
            indices.push_back(vert);

            vert++;
        }

        vertices.push_back(0.f);
        vertices.push_back(-radius);
        vertices.push_back(0.f);

        int vertexcount = vertices.size() / 3;

        // faces for lower sphere fan cap
        for (vert = vertexcount - slices; vert < vertexcount - 1; vert++)
        {
            indices.push_back(vert - 1);
            indices.push_back(vertexcount - 1);
            indices.push_back(vert);
        }
        indices.push_back(vertexcount - 1 - slices);
        indices.push_back(vertexcount - 2);
        indices.push_back(vertexcount - 1);
    }
}