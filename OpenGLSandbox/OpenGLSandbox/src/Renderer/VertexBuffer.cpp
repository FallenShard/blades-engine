#include <fstream>
#include <cassert>
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

void VertexBuffer::release(GLenum targetType)
{
    glBindBuffer(targetType, 0);
}


void VertexBuffer::create(GLfloat* vertices, int size, int packSize)
{
    m_packSize = packSize;

    for (int i = 0; i < size; i++)
        m_vertices.push_back(vertices[i]);

    glBufferData(m_targetType, sizeof(GLfloat) * m_vertices.size(), m_vertices.data(), m_drawType);
}

void VertexBuffer::create(std::vector<GLfloat> vertices, int packSize)
{
    m_packSize = packSize;

    m_vertices = vertices;

    glBufferData(m_targetType, sizeof(GLfloat) * m_vertices.size(), m_vertices.data(), m_drawType);
}

GLsizei VertexBuffer::getSize() const
{
    return m_vertices.size();
}

void VertexBuffer::loadFromFile(std::string fileName, int packSize)
{
    m_packSize = packSize;

    std::ifstream inputFile(fileName, std::ios::in);

    float vertexData;
    while (inputFile >> vertexData)
    {
        m_vertices.push_back(vertexData);
    }

    glBufferData(m_targetType, sizeof(GLfloat) * m_vertices.size(), m_vertices.data(), m_drawType);
}

GLfloat& VertexBuffer::operator[](unsigned int index)
{
    assert(index >= 0 && index < m_vertices.size());
    
    return m_vertices[index];
}
