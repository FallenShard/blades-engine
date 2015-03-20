#include "Meshes/PlaneMesh.h"

#include "OglWrapper/VertexStreams.h"

namespace fsi
{

PlaneMesh::PlaneMesh(unsigned int xSquares, unsigned int zSquares, float squareSize)
    : m_xSquares(xSquares)
    , m_zSquares(zSquares)
    , m_squareSize(squareSize)
{
    m_xTotalSize = m_xSquares * m_squareSize;
    m_zTotalSize = m_zSquares * m_squareSize;

    m_vertexBuffer = std::make_unique<VertexBuffer>(GL_STATIC_DRAW);
    m_indexBuffer = std::make_unique<IndexBuffer>();

    initVertices();
}

PlaneMesh::~PlaneMesh()
{
}

void PlaneMesh::bind()
{
    m_vertexBuffer->bind(VertexBufferBinding::Position, 0, sizeof(GLfloat) * 3);
    m_indexBuffer->bind();
}

void PlaneMesh::initVertices()
{
    for (unsigned int z = 0; z < m_zSquares + 1; z++)
    {
        for (unsigned int x = 0; x < m_xSquares + 1; x++)
        {
            m_vertexBuffer->push(x * m_squareSize - m_xTotalSize / 2, 0.f, z * m_squareSize - m_zTotalSize / 2);
        }
    }

    for (unsigned int z = 0; z < m_zSquares; z++)
    {
        for (unsigned int x = 0; x < m_xSquares; x++)
        {
            m_indexBuffer->push(z * (m_xSquares + 1) + x, z * (m_xSquares + 1) + x + 1, (z + 1) * (m_xSquares + 1) + x + 1, (z + 1) * (m_xSquares + 1) + x);
        }
    }

    m_vertexBuffer->bind();
    m_vertexBuffer->uploadData();
    VertexBuffer::release();

    m_indexBuffer->bind();
    m_indexBuffer->uploadData();
    IndexBuffer::release();

    m_vertexBuffer->setVertexSize(3);

    m_vertexAttributes.emplace_back(VertexAttrib::Position, 3, GL_FLOAT, GL_FALSE, 0);
}

unsigned int PlaneMesh::getVertexCount()
{
    return m_vertexBuffer->getVertexCount();
}

unsigned int PlaneMesh::getIndexCount()
{
    return m_indexBuffer->getSize();
}

}