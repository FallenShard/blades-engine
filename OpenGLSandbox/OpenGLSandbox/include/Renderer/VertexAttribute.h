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

    VertexAttribute()
    {
    }

    VertexAttribute(std::string name, GLint perVertexData, GLsizei stride, GLint offset)
    {
        this->name = name;
        this->size = perVertexData;
        this->type = GL_FLOAT;
        this->normalized = GL_FALSE;
        this->stride = stride;
        this->offset = (const GLvoid*)offset;
    }

    VertexAttribute(std::string name, GLint perVertexData, GLenum type, GLboolean normalized, GLsizei stride, GLint offset)
    {
        this->name = name;
        this->size = perVertexData;
        this->type = type;
        this->normalized = normalized;
        this->stride = stride;
        this->offset = (const GLvoid *)offset;
    }

    void locate(GLuint programId)
    {
        location = glGetAttribLocation(programId, name.c_str());
    }

    void enable()
    {
        glEnableVertexAttribArray(location);
        glVertexAttribPointer(location, size, type, normalized, stride, offset);
    }

    void enable(GLuint programId)
    {
        location = glGetAttribLocation(programId, name.c_str());
        glEnableVertexAttribArray(location);
        glVertexAttribPointer(location, size, type, normalized, stride, offset);
    }

    void disable()
    {
        glDisableVertexAttribArray(location);
    }
};

#endif // VERTEX_ATTRIBUTE_H