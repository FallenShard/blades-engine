#include <fstream>
#include <cassert>
#include "OglWrapper/IndexBuffer.h"

namespace
{
    GLuint boundIBO = 0;
}

namespace fsi
{

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
    glDeleteBuffers(1, &m_id);
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

    uploadData();
}

void IndexBuffer::create(std::vector<GLshort> indices)
{
    m_indices = indices;

    uploadData();
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

    inputFile.close();

    uploadData();
}

void IndexBuffer::push(GLshort p1, GLshort p2, GLshort p3)
{
    m_indices.emplace_back(p1);
    m_indices.emplace_back(p2);
    m_indices.emplace_back(p3);
}

void IndexBuffer::push(GLshort p1, GLshort p2, GLshort p3, GLshort p4)
{
    m_indices.emplace_back(p1);
    m_indices.emplace_back(p2);
    m_indices.emplace_back(p3);
    m_indices.emplace_back(p4);
}

void IndexBuffer::uploadData()
{
    glBufferData(m_targetType, sizeof(GLshort) * m_indices.size(), m_indices.data(), m_usageType);
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

}
