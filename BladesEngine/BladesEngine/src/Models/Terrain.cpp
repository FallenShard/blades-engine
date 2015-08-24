#include <fstream>
#include <iostream>
#include <vector>

#include "Renderer/GLRenderer.h"
#include "Renderer/Technique.h"

#include "Models/Terrain.h"

namespace fsi
{
    Terrain::Terrain(int tiles, float tileSize, float worldScale, GLRenderer* renderer)
        : m_technique(nullptr)
        , m_triangleSize(8.f)
        , m_isWireframe(false)
    {
        auto vertexData = generateVertices(tiles, tileSize);
        auto indices = generateIndices(tiles);

        int vertexCount = vertexData.size() / 3;
        int indexCount = indices.size();

        auto bufferManager = renderer->getDeviceBufferManager();
        GLuint vbo = bufferManager->allocate(vertexData.size() * sizeof(GLfloat), GL_MAP_WRITE_BIT);
        bufferManager->update(vbo, vertexData);
        GLuint ibo = bufferManager->allocate(indices.size() * sizeof(GLushort), GL_MAP_WRITE_BIT);
        bufferManager->update(ibo, indices);

        VertexLayout layout;
        layout.indexBuffer = ibo;
        layout.vertexBuffers.emplace_back(0, BufferDescriptor{ vbo, 0, 3 * sizeof(GLfloat) });
        layout.attributes.emplace_back(0, AttributeFormat{ VertexAttrib::Position, 3, 0 });
        auto vao = renderer->getVertexAssembly()->createInputState(layout);

        renderer->setTessellationPatchVertices(4);
        
        auto texManager = renderer->getTextureManager();
        TextureHandle heightMap      = texManager->loadTexture("heightmap1024.png", 1, InternalFormat::R8, BaseFormat::Red);
        TextureHandle patchHeightMap = texManager->loadTexture("perlinHmap.png", 1, InternalFormat::R8, BaseFormat::Red);
        TextureHandle globalTex      = texManager->loadTexture("lunar.png", 8, InternalFormat::RGBA8, BaseFormat::RGBA);
        TextureHandle noiseTex       = texManager->loadTexture("noise.png", 8, InternalFormat::R8, BaseFormat::Red);
        TextureHandle patchTex       = texManager->loadTexture("seamlessmoon.png", 8, InternalFormat::RGBA8, BaseFormat::RGBA);

        SamplerHandle linClampSampler = texManager->getSamplerPreset(TextureManager::LinearClamp);
        SamplerHandle linMirrSampler  = texManager->getSamplerPreset(TextureManager::LinearMirrored);
        SamplerHandle mipmapSampler   = texManager->createSampler(Filter::Mipmap, Filter::Linear, WrapMode::Repeat, WrapMode::Repeat);

        auto heightMapInfo      = texManager->createTextureInfo(heightMap, linClampSampler);
        auto patchHeightMapInfo = texManager->createTextureInfo(patchHeightMap, linMirrSampler);
        auto globalTexInfo      = texManager->createTextureInfo(globalTex, mipmapSampler);
        auto noiseTexInfo       = texManager->createTextureInfo(noiseTex, mipmapSampler);
        auto patchTexInfo       = texManager->createTextureInfo(patchTex, mipmapSampler);

        auto techId = renderer->getTechniqueCache()->getProgram("terrain");
        m_technique = std::make_unique<Technique>(techId);

        m_technique->setUniformAttribute("heightMap"      , heightMapInfo.unit);
        m_technique->setUniformAttribute("detailHeightMap", patchHeightMapInfo.unit);
        m_technique->setUniformAttribute("terrainTexture" , globalTexInfo.unit);
        m_technique->setUniformAttribute("noiseMap"       , noiseTexInfo.unit);
        m_technique->setUniformAttribute("patchTexture"   , patchTexInfo.unit);

        m_technique->setUniformAttribute("worldScale", worldScale);
        m_technique->setUniformAttribute("triSize"   , m_triangleSize);
        m_technique->setUniformAttribute("screenSize", glm::vec2(renderer->getScreenSize()));
        m_technique->setUniformAttribute("wireframe", 0);

        TerrainParams terrainParams;
        terrainParams.heightFactor = tiles / 16.f;
        terrainParams.detailFactor = terrainParams.heightFactor * 0.01f;
        terrainParams.patches = static_cast<float>(tiles);
        terrainParams.terrainSize = static_cast<float>(tiles * tileSize * worldScale);

        auto ubo = bufferManager->allocate(sizeof(terrainParams), GL_MAP_WRITE_BIT);
        bufferManager->update(ubo, &terrainParams, sizeof(terrainParams));

        auto uboDesc = m_technique->createUniformBufferDescriptor("TerrainParams", 0, ubo);

        m_drawItem.program = techId;
        m_drawItem.primitiveType = GL_PATCHES;
        m_drawItem.vertexArray = vao;
        m_drawItem.numVerts = vertexCount;
        m_drawItem.numIndices = indexCount;
        m_drawItem.baseVertex = 0;
        m_drawItem.preRender = [this, worldScale](const glm::mat4& P, const glm::mat4& V)
        {
            m_technique->setUniformAttribute("P", P);
            
            glm::mat4 model = glm::translate(glm::vec3(0.f, -worldScale * 10.f, 0.f)) * glm::scale(glm::vec3(worldScale));
            glm::mat4 modelView = V * model;
            m_technique->setUniformAttribute("MV", modelView);

            glm::mat4 mat = P * modelView;
            m_technique->setUniformAttribute("MVP", mat);

            glm::mat3 normalMatrix = glm::mat3(glm::inverseTranspose(modelView));
            m_technique->setUniformAttribute("normalMatrix", normalMatrix);

            glm::vec4 lightDir = glm::normalize(V * glm::normalize(glm::vec4(1.f, 1.f, 0.f, 0.f)));
            m_technique->setUniformAttribute("mvLightDir", lightDir);
        };
        m_drawItem.postRender = nullptr;

        renderer->submitDrawItem(m_drawItem);
    }

    Terrain::~Terrain()
    {
    }

    void Terrain::setWireframe(bool isWireframe)
    {
        m_isWireframe = isWireframe;
        m_technique->setUniformAttribute("wireframe", m_isWireframe);
    }

    void Terrain::setTriSize(int value)
    {
        m_triangleSize = static_cast<GLfloat>(value);
        m_technique->setUniformAttribute("triSize", m_triangleSize);
    }

    std::vector<GLfloat> Terrain::generateVertices(int tiles, float tileSize)
    {
        std::vector<GLfloat> vertices;
        
        for (int z = 0; z < tiles + 1; z++)
        {
            for (int x = 0; x < tiles + 1; x++)
            {
                vertices.push_back(x * tileSize - tiles * tileSize / 2.f);
                vertices.push_back(0.f);
                vertices.push_back(tiles * tileSize / 2.f - z * tileSize);
            }
        }

        return vertices;
    }

    std::vector<GLushort> Terrain::generateIndices(int tiles)
    {
        std::vector<GLushort> indices;

        for (int z = 0; z < tiles; z++)
        {
            for (int x = 0; x < tiles; x++)
            {
                indices.push_back(z * (tiles + 1) + x);
                indices.push_back(z * (tiles + 1) + x + 1);
                indices.push_back((z + 1) * (tiles + 1) + x + 1);
                indices.push_back((z + 1) * (tiles + 1) + x);
            }
        }

        return indices;
    }
}