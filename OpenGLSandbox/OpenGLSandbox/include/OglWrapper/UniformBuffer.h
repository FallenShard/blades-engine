#pragma once

#include <vector>
#include "OpenGL.h"

class ShaderProgram;

class UniformBuffer
{
public:
    UniformBuffer();
    UniformBuffer(ShaderProgram* program);

    ~UniformBuffer();

    void setBufferData(GLvoid* data, GLsizei bufferSize);

    void setBindingPoint(int bindingPoint);

    void bind() const;
    void bindToBindingPoint() const;

    static void release();

private:
    GLuint  m_id;

    GLvoid* m_data;
    GLsizei m_size;

    GLint   m_bindingPoint;
};