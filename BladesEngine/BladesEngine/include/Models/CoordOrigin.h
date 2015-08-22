#pragma once

#include <vector>
#include <memory>

#include "Renderer/DrawItem.h"

namespace fsi
{
    class GLRenderer;
    class Technique;

    class CoordOrigin
    {
    public:
        CoordOrigin(float size, GLRenderer* renderer);
        ~CoordOrigin();

    private:
        std::vector<GLfloat> generateVertices(float size);

        std::unique_ptr<Technique> m_technique;
        DrawItem m_drawItem;
    };
}