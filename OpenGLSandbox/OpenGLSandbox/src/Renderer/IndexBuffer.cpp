#include <fstream>
#include <cassert>
#include "Renderer/IndexBuffer.h"


IndexBuffer::IndexBuffer(GLenum targetType, GLenum drawType)
    : m_targetType(targetType)
    , m_usageType(drawType)
{
    glGenBuffers(1, &m_id);
}

IndexBuffer::~IndexBuffer()
{
}

void IndexBuffer::bind() const
{
    glBindBuffer(m_targetType, m_id);
}

void IndexBuffer::release(GLenum targetType)
{
    glBindBuffer(targetType, 0);
}

void IndexBuffer::release(const IndexBuffer& buffer)
{
    glBindBuffer(buffer.m_targetType, 0);
}

void IndexBuffer::create(GLshort* vertices, int size)
{
    for (int i = 0; i < size; i++)
        m_indices.push_back(vertices[i]);

    glBufferData(m_targetType, sizeof(GLshort) * m_indices.size(), m_indices.data(), m_usageType);
}

void IndexBuffer::create(std::vector<GLshort> indices)
{
    m_indices = indices;

    glBufferData(m_targetType, sizeof(GLshort) * m_indices.size(), m_indices.data(), m_usageType);
}

GLsizei IndexBuffer::getSize() const
{
    return m_indices.size();
}

void IndexBuffer::loadFromFile(std::string fileName)
{
    std::ifstream inputFile(fileName, std::ios::in);

    // Load the index data
    GLshort indexData;
    while (inputFile >> indexData)
    {
        m_indices.push_back(indexData);
    }

    glBufferData(m_targetType, sizeof(GLfloat) * m_indices.size(), m_indices.data(), m_usageType);
}

GLshort& IndexBuffer::operator[](unsigned int index)
{
    assert(index >= 0 && index < m_indices.size());

    return m_indices[index];
}
