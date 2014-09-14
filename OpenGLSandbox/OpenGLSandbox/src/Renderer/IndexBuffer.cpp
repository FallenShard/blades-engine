#include <fstream>
#include <cassert>
#include "Renderer/IndexBuffer.h"

namespace
{
    GLuint boundIBO = 0;
}

IndexBuffer::IndexBuffer()
    : m_targetType(GL_ELEMENT_ARRAY_BUFFER)
    , m_usageType(GL_STATIC_DRAW)
{
    glGenBuffers(1, &m_id);
}

IndexBuffer::IndexBuffer(GLenum usageType)
    : m_targetType(GL_ELEMENT_ARRAY_BUFFER)
    , m_usageType(usageType)
{
    glGenBuffers(1, &m_id);
}

IndexBuffer::IndexBuffer(GLenum targetType, GLenum usageType)
    : m_targetType(targetType)
    , m_usageType(usageType)
{
    glGenBuffers(1, &m_id);
}

IndexBuffer::~IndexBuffer()
{
}

void IndexBuffer::bind() const
{
    if (boundIBO != m_id)
    {
        glBindBuffer(m_targetType, m_id);
        boundIBO = m_id;
    }
}

void IndexBuffer::release()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    boundIBO = 0;
}

void IndexBuffer::release(GLenum targetType)
{
    glBindBuffer(targetType, 0);
    boundIBO = 0;
}

void IndexBuffer::release(const IndexBuffer& buffer)
{
    glBindBuffer(buffer.m_targetType, 0);
    boundIBO = 0;
}

void IndexBuffer::create(GLshort* vertices, unsigned int size)
{
    m_indices.clear();

    for (unsigned int i = 0; i < size; i++)
        m_indices.push_back(vertices[i]);

    glBufferData(m_targetType, sizeof(GLshort) * m_indices.size(), m_indices.data(), m_usageType);
}

void IndexBuffer::create(std::vector<GLshort> indices)
{
    m_indices = indices;

    glBufferData(m_targetType, sizeof(GLshort) * m_indices.size(), m_indices.data(), m_usageType);
}

void IndexBuffer::loadFromFile(std::string fileName)
{
    std::ifstream inputFile(fileName, std::ios::in);

    // Load the index data
    m_indices.clear();
    GLshort indexData;

    while (inputFile >> indexData)
    {
        m_indices.push_back(indexData);
    }

    glBufferData(m_targetType, sizeof(GLfloat) * m_indices.size(), m_indices.data(), m_usageType);
}

GLsizei IndexBuffer::getSize() const
{
    return m_indices.size();
}

GLshort& IndexBuffer::operator[](unsigned int index)
{
    assert(index >= 0 && index < m_indices.size());

    return m_indices[index];
}
