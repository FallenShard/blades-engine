#pragma once

#include <functional>

#include "OpenGL.h"

namespace fsi
{
    struct DrawItem
    {
        GLuint program;
        GLuint vertexArray;

        GLenum primitiveType;
        GLsizei numVerts;
        GLsizei numIndices;
        GLuint baseVertex;

        std::function<void(const glm::mat4& P, const glm::mat4& V)> preRender;
        std::function<void()> postRender;
    };
}
