#pragma once

#include "OpenGL.h"

namespace fsi
{
    class RenderPass
    {
    public:
        virtual void setAsSurface() = 0;
        virtual void render() = 0;
        virtual void resize(int width, int height) = 0;

        virtual ~RenderPass() = default;

        static void clearScreen();

    private:
        static const GLfloat defaultDepth;
        static const GLfloat defaultColor[4];
    };
}