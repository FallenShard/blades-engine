#ifndef VERTEX_ATTRIBUTE_H
#define VERTEX_ATTRIBUTE_H

#include <string>
#include "OpenGL.h"

struct VertexAttribute
{
    std::string name;
    GLuint location;
    GLint size;
    GLenum type;
    GLboolean normalized;
    GLsizei stride;
    const GLvoid* offset;

    void enable()
    {
        glEnableVertexAttribArray(location);
        glVertexAttribPointer(location, size, type, normalized, stride, offset);
    }
};

#endif