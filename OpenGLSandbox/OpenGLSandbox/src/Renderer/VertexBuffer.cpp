#include <fstream>
#include <cassert>
#include "Renderer/VertexBuffer.h"

VertexBuffer::VertexBuffer(GLenum targetType, GLenum drawType)
    : m_targetType(targetType)
    , m_usageType(drawType)
    , m_dataPerVertex(0)
{
    glGenBuffers(1, &m_id);
}

VertexBuffer::~VertexBuffer()
{
}

void VertexBuffer::bind() const
{
    glBindBuffer(m_targetType, m_id);
}

void VertexBuffer::release(GLenum targetType)
{
    glBindBuffer(targetType, 0);
}

void VertexBuffer::release(const VertexBuffer& buffer)
{
    glBindBuffer(buffer.m_targetType, 0);
}

void VertexBuffer::create(GLfloat* vertices, unsigned int size)
{
    for (unsigned int i = 0; i < size; i++)
        m_vertexData.push_back(vertices[i]);

    glBufferData(m_targetType, sizeof(GLfloat) * m_vertexData.size(), m_vertexData.data(), m_usageType);
}

void VertexBuffer::create(std::vector<GLfloat> vertices)
{
    m_vertexData = vertices;

    glBufferData(m_targetType, sizeof(GLfloat) * m_vertexData.size(), m_vertexData.data(), m_usageType);
}

void VertexBuffer::loadFromFile(std::string fileName)
{
    std::ifstream inputFile(fileName, std::ios::in);

    // Load the vertex data
    GLfloat vertexData;
    while (inputFile >> vertexData)
    {
        m_vertexData.push_back(vertexData);
    }

    glBufferData(m_targetType, sizeof(GLfloat) * m_vertexData.size(), m_vertexData.data(), m_usageType);
}

void VertexBuffer::registerAttributeSize(int dataPerVertexAttribute)
{
    m_dataPerVertex += dataPerVertexAttribute;
}

GLsizei VertexBuffer::getSize() const
{
    return m_vertexData.size();
}

GLsizei VertexBuffer::getVertexAmount() const
{
    if (m_dataPerVertex == 0)
        return -1;

    return m_vertexData.size() / m_dataPerVertex;
}

GLfloat& VertexBuffer::operator[](unsigned int index)
{
    assert(index >= 0 && index < m_vertexData.size());
    
    return m_vertexData[index];
}
