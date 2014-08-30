#include <fstream>
#include <cassert>

template<typename T>
VertexBuffer<T>::VertexBuffer(GLenum targetType, GLenum drawType)
    : m_targetType(targetType)
    , m_usageType(drawType)
    , m_vertexNumber(0)
{
    create();
}

template<typename T>
VertexBuffer<T>::~VertexBuffer()
{
}

template<typename T>
void VertexBuffer<T>::create()
{
    glGenBuffers(1, &m_id);
}

template<typename T>
void VertexBuffer<T>::bind() const
{
    glBindBuffer(m_targetType, m_id);
}

template<typename T>
void VertexBuffer<T>::release(GLenum targetType)
{
    glBindBuffer(targetType, 0);
}

template<typename T>
void VertexBuffer<T>::release(const VertexBuffer<T>& vertexBuffer)
{
    glBindBuffer(vertexBuffer.m_targetType, 0);
}

template<typename T>
void VertexBuffer<T>::create(T* vertices, int size, int packSize)
{
    m_packSize = packSize;

    for (int i = 0; i < size; i++)
        m_vertexData.push_back(vertices[i]);

    m_vertexNumber = size / (sizeof(T) * packSize);

    glBufferData(m_targetType, sizeof(T) * m_vertexData.size(), m_vertexData.data(), m_usageType);
}

template<typename T>
void VertexBuffer<T>::create(std::vector<T> vertices, int packSize)
{
    m_packSize = packSize;
    m_vertices = vertices;
    m_vertexNumber = vertices.size() / (sizeof(T) * packSize);

    glBufferData(m_targetType, sizeof(T) * m_vertexData.size(), m_vertexData.data(), m_usageType);
}

template<typename T>
GLsizei VertexBuffer<T>::getSize() const
{
    return m_vertexNumber;
}

template<typename T>
void VertexBuffer<T>::loadFromFile(std::string fileName, int packSize)
{
    m_packSize = packSize;

    std::ifstream inputFile(fileName, std::ios::in);

    // Read in number of vertices
    inputFile >> m_vertexNumber;

    // Load the vertex data
    T vertexData;
    while (inputFile >> vertexData)
    {
        m_vertexData.push_back(vertexData);
    }

    glBufferData(m_targetType, sizeof(T) * m_vertexData.size(), m_vertexData.data(), m_usageType);
}

template<typename T>
T& VertexBuffer<T>::operator[](unsigned int index)
{
    assert(index >= 0 && index < m_vertices.size());

    return m_vertices[index];
}
