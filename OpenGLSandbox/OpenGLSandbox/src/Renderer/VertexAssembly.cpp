#include "Renderer/VertexAssembly.h"

namespace fsi
{
    VertexAssembly::VertexAssembly()
    {
    }

    VertexAssembly::~VertexAssembly()
    {
        for (auto vertexArray : m_vertexArrays)
            glDeleteVertexArrays(1, &vertexArray);
    }

    GLuint VertexAssembly::createInputState(const VertexLayout& layout)
    {
        GLuint vertexArray;
        glCreateVertexArrays(1, &vertexArray);
        m_vertexArrays.push_back(vertexArray);

        if (layout.indexBuffer != 0)
            glVertexArrayElementBuffer(vertexArray, layout.indexBuffer);

        for (const auto& vb : layout.vertexBuffers)
            glVertexArrayVertexBuffer(vertexArray, vb.first, 
                vb.second.id, vb.second.offset, vb.second.stride);

        for (const auto& attrib : layout.attributes)
        {
            glEnableVertexArrayAttrib(vertexArray, attrib.second.index);
            glVertexArrayAttribBinding(vertexArray, attrib.second.index, attrib.first);
            glVertexArrayAttribFormat(vertexArray, attrib.second.index, attrib.second.size,
                GL_FLOAT, GL_FALSE, attrib.second.offset);
        }

        return vertexArray;
    }
}