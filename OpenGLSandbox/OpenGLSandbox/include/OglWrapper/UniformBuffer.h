#pragma once

#include <vector>

class ShaderProgram;

class UniformBuffer
{
public:
    UniformBuffer(ShaderProgram* program);

    ~UniformBuffer();

    void bind() const;

    static void release();

private:
    //GLvoid* m_data;
    //GLsizei m_size;
};