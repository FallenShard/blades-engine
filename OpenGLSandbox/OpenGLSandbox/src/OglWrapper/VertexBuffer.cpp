#include <fstream>
#include <sstream>
#include <cassert>
#include "OglWrapper/VertexBuffer.h"

namespace
{
    GLuint boundVBO = 0;
}

namespace fsi
{

VertexBuffer::VertexBuffer(GLenum usageType)
    : m_targetType(GL_ARRAY_BUFFER)
    , m_usageType(usageType)
    , m_vertexSize(0)
    , m_numVertices(0)
{
    glGenBuffers(1, &m_id);
}

VertexBuffer::VertexBuffer(GLenum targetType, GLenum usageType)
    : m_targetType(targetType)
    , m_usageType(usageType)
    , m_vertexSize(0)
    , m_numVertices(0)
{
    glGenBuffers(1, &m_id);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_id);
}

void VertexBuffer::bind() const
{
    if (boundVBO != m_id)
    {
        glBindBuffer(m_targetType, m_id);
        boundVBO = m_id;
    }
}

void VertexBuffer::bind(GLuint bindingIndex, GLintptr offset, GLsizei stride) const
{
    glBindVertexBuffer(bindingIndex, m_id, offset, stride);
}

void VertexBuffer::release()
{
    if (boundVBO != 0)
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        boundVBO = 0;
    }
}

void VertexBuffer::release(GLenum targetType)
{
    glBindBuffer(targetType, 0);
    boundVBO = 0;
}

void VertexBuffer::release(const VertexBuffer& buffer)
{
    glBindBuffer(buffer.m_targetType, 0);
    boundVBO = 0;
}

void VertexBuffer::create(GLfloat* vertices, unsigned int size)
{
    m_vertexData.clear();

    for (unsigned int i = 0; i < size; i++)
        m_vertexData.push_back(vertices[i]);

    uploadData();
}

void VertexBuffer::create(std::vector<GLfloat> vertices)
{
    m_vertexData = vertices;

    uploadData();
}

void VertexBuffer::loadFromFile(std::string fileName)
{
    std::ifstream inputFile(fileName, std::ios::in);
    std::string line;

    m_vertexData.clear();
    GLfloat vertexData;

    // Read the file lines one by one
    while (std::getline(inputFile, line))
    {
        std::stringstream lineStream(line);

        while (lineStream >> vertexData)
            m_vertexData.push_back(vertexData);
    }

    uploadData();
}

void VertexBuffer::push(GLfloat value)
{
    m_vertexData.emplace_back(value);
}

void VertexBuffer::push(GLfloat x, GLfloat y)
{
    m_vertexData.emplace_back(x);
    m_vertexData.emplace_back(y);
}

void VertexBuffer::push(GLfloat x, GLfloat y, GLfloat z)
{
    m_vertexData.emplace_back(x);
    m_vertexData.emplace_back(y);
    m_vertexData.emplace_back(z);
}

void VertexBuffer::push(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    m_vertexData.emplace_back(x);
    m_vertexData.emplace_back(y);
    m_vertexData.emplace_back(z);
    m_vertexData.emplace_back(w);
}

void VertexBuffer::uploadData()
{
    glBufferData(m_targetType, sizeof(GLfloat) * m_vertexData.size(), m_vertexData.data(), m_usageType);
}

void VertexBuffer::clear()
{
    m_vertexData.clear();
}

void VertexBuffer::setVertexSize(GLsizei vertexSize)
{
    m_vertexSize = vertexSize;
    m_numVertices = m_vertexData.size() / m_vertexSize;
}

GLsizei VertexBuffer::getVertexCount() const
{
    return m_numVertices;
}

GLfloat& VertexBuffer::operator[](unsigned int index)
{
    assert(index >= 0 && index < m_vertexData.size());
    
    return m_vertexData[index];
}

}