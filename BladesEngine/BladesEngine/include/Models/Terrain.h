#pragma once

#include <vector>
#include <memory>

#include "OpenGL.h"

#include "Renderer/DrawItem.h"

namespace fsi
{
    class GLRenderer;
    class Technique;

    class Terrain
    {
    public:
        Terrain(int tiles, float tileSize, float worldScale, GLRenderer* renderer);
        ~Terrain();

        void setWireframe(bool isWireframe);
        void setTriSize(int value);

    private:
        struct TerrainParams
        {
            float detailFactor;
            float heightFactor;
            float patches;
            float terrainSize;
        };

        std::vector<GLfloat> generateVertices(int tiles, float tileSize);
        std::vector<GLushort> generateIndices(int tiles);

        std::unique_ptr<Technique> m_technique;

        GLfloat m_triangleSize;
        GLint m_isWireframe;

        DrawItem m_drawItem;
    };
}