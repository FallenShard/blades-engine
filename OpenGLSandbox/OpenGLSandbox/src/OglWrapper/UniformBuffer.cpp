#include "OglWrapper/UniformBuffer.h"
#include "OglWrapper/ShaderProgram.h"

namespace
{
    GLuint prevBoundId = 0;
}

namespace fsi
{

UniformBuffer::UniformBuffer()
{
    glGenBuffers(1, &m_id);
}

UniformBuffer::UniformBuffer(ShaderProgram* program)
{

}

UniformBuffer::~UniformBuffer()
{

}

void UniformBuffer::setBufferData(GLvoid* data, GLsizei bufferSize)
{
    m_data = data;
    m_size = bufferSize;
    glBufferData(GL_UNIFORM_BUFFER, m_size, m_data, GL_DYNAMIC_DRAW);
}

void UniformBuffer::setBindingPoint(int bindingPoint)
{
    m_bindingPoint = bindingPoint;
}

void UniformBuffer::bind() const
{
    if (prevBoundId != m_id)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, m_id);
        prevBoundId = m_id;
    }
}

void UniformBuffer::bindToBindingPoint() const
{
    glBindBufferBase(GL_UNIFORM_BUFFER, m_bindingPoint, m_id);
}

void UniformBuffer::release()
{
    if (prevBoundId != 0)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        prevBoundId = 0;
    }
}

}