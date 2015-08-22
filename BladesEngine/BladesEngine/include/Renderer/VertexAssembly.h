#pragma once

#include <map>
#include <vector>
#include "OpenGL.h"

namespace fsi
{
    namespace VertexAttrib
    {
        enum ID
        {
            Position = 0,
            Normal = 1,
            TexCoord0 = 2,
            Color = 3,
            Count
        };
    }

    namespace VertexBufferBinding
    {
        enum ID
        {
            Slot0 = 0,
            Slot1 = 1,
            Slot2 = 2,
            Slot3 = 3,
            Count
        };
    }

    struct AttributeFormat
    {
        GLuint index;
        GLsizei size;
        GLuint offset;
    };

    struct BufferDescriptor
    {
        GLuint id;
        GLintptr offset;
        GLsizei stride;
    };

    struct VertexLayout
    {
        GLuint indexBuffer;
        std::vector<std::pair<GLuint, BufferDescriptor>> vertexBuffers;
        std::vector<std::pair<GLuint, AttributeFormat>> attributes;
    };

    class VertexAssembly
    {
    public:

        VertexAssembly();
        ~VertexAssembly();

        GLuint createInputState(const VertexLayout& layout);

    private:
        std::vector<GLuint> m_vertexArrays;

    };
}