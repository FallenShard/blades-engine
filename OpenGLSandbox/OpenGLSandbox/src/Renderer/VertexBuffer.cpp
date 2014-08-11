#include "Renderer/VertexBuffer.h"

VertexBuffer::VertexBuffer(GLenum targetType, GLenum drawType)
    : m_targetType(targetType)
    , m_drawType(drawType)
{
    create();
}

VertexBuffer::~VertexBuffer()
{
}

void VertexBuffer::create()
{
    glGenBuffers(1, &m_id);
}

void VertexBuffer::bind() const
{
    glBindBuffer(m_targetType, m_id);
}

void VertexBuffer::setData(GLfloat* vertices, int size, int packSize)
{
    m_packSize = packSize;

    for (int i = 0; i < size; i++)
        m_vertices.push_back(vertices[i]);

    glBufferData(m_targetType, sizeof(GLfloat) * size, m_vertices.data(), m_drawType);
}

void VertexBuffer::setData(std::vector<GLfloat> vertices, int packSize)
{
    m_packSize = packSize;

    m_vertices = vertices;

    glBufferData(m_targetType, sizeof(GLfloat) * m_vertices.size(), m_vertices.data(), m_drawType);
}

GLsizei VertexBuffer::getSize() const
{
    return m_vertices.size();
}
