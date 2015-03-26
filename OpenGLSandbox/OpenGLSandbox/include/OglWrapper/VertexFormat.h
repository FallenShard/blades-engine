//glVertexAttribFormat(attribute.location, attribute.size, attribute.type, attribute.normalized, attribute.iOffset);

#pragma once

#include "OpenGL.h"
#include "VertexStreams.h"

namespace fsi
{

struct VertexFormat
{
    VertexAttrib::ID index;
    GLint size;
    GLenum type;
    GLboolean normalized;
    GLuint offset;

    VertexFormat(VertexAttrib::ID index, GLint size, GLenum type, GLboolean normalized, GLuint offset)
        : index(index)
        , size(size)
        , type(type)
        , normalized(normalized)
        , offset(offset)
    {
    }

    VertexFormat(VertexAttrib::ID index, GLint size, GLenum type, GLuint offset)
        : index(index)
        , size(size)
        , type(type)
        , normalized(GL_FALSE)
        , offset(offset)
    {
    }

    bool operator<(const VertexFormat& other) const
    {
        return index < other.index;
    }
};

}
