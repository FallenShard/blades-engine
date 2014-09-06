#include <fstream>
#include <sstream>
#include <cassert>
#include <algorithm>
#include "Renderer/VertexBuffer.h"

VertexBuffer::VertexBuffer(GLenum targetType, GLenum drawType)
    : m_targetType(targetType)
    , m_usageType(drawType)
    , m_vertexCount(0)
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
    std::string line;

    // Load the vertex data
    int positionDataCount = 0;
    int colorDataCount = 0;
    int perVertexDataCount = 0;

    // Read the file lines one by one and register the attributes
    while (std::getline(inputFile, line))
    {
        positionDataCount = loadAttributeFromFile(line, "Vertices", inputFile);
        perVertexDataCount += positionDataCount;

        colorDataCount = loadAttributeFromFile(line, "Colors", inputFile);
        perVertexDataCount += colorDataCount;
    }

    m_vertexCount = m_vertexData.size() / perVertexDataCount;

    registerAttribute("Position", positionDataCount, 0);

    GLint colorOffset = m_vertexCount * positionDataCount * sizeof GLfloat;
    registerAttribute("Color", colorDataCount, colorOffset);

    glBufferData(m_targetType, sizeof(GLfloat) * m_vertexData.size(), m_vertexData.data(), m_usageType);
}

int VertexBuffer::loadAttributeFromFile(std::string& line, std::string attribute, std::ifstream& stream)
{
    GLfloat vertexData;
    int attributeDataCount = 0;
    int dataCounter = 0;

    // Try to find attribute header
    if (line.find(attribute) != std::string::npos)
    {
        // If so, continue reading file line by line
        while (std::getline(stream, line))
        {
            std::stringstream lineStream(line);

            // Read all coordinates one by one per line
            dataCounter = 0;
            while (lineStream >> vertexData)
            {
                m_vertexData.push_back(vertexData);
                dataCounter++;
            }

            // Remember the highest seen per vertex attribute data
            attributeDataCount = std::max(attributeDataCount, dataCounter);

            // End reading session if there's no data anymore for this attribute
            if (dataCounter == 0 && line != "")
                break;
        }
    }

    return attributeDataCount;
}

void VertexBuffer::registerAttribute(std::string name, GLint dataPerAttribute, GLint offset)
{
    if (dataPerAttribute != 0)
    {
        VertexAttribute attribute(name, dataPerAttribute, GL_FLOAT, GL_FALSE, 0, offset);
        m_vertexAttributes[name] = std::make_shared<VertexAttribute>(attribute);
    }
}

GLsizei VertexBuffer::getSize() const
{
    return m_vertexData.size();
}

GLsizei VertexBuffer::getVertexCount() const
{
    if (m_vertexCount == 0)
        return -1;

    return m_vertexCount;
}

GLfloat& VertexBuffer::operator[](unsigned int index)
{
    assert(index >= 0 && index < m_vertexData.size());
    
    return m_vertexData[index];
}

VertexBuffer::AttributeMap& VertexBuffer::getAttributeMap()
{
    return m_vertexAttributes;
}
