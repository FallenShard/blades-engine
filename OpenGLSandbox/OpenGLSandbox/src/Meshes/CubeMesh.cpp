#include "Meshes/CubeMesh.h"
#include "Utils/VertexLoader.h"

namespace fsi
{

CubeMesh::CubeMesh()
    : m_vertexBuffer(GL_STATIC_DRAW)
{
    init();
}

CubeMesh::~CubeMesh()
{
}

void CubeMesh::init()
{
    m_vertexArray.bind();

    m_vertexBuffer.bind();
    VertexLoader vLoader;
    vLoader.loadFromFile("res/CubeMesh.txt", m_vertexBuffer);
    m_vertexArray.setVertexCount(m_vertexBuffer.getVertexCount());

    m_indexBuffer.bind();
    m_indexBuffer.loadFromFile("res/RobotArmFaces.txt");
    m_vertexArray.attachIndexBuffer(&m_indexBuffer);

    m_vertexArray.attachAttribute(VertexAttribute("position", 3, 0, 0));
    m_vertexArray.attachAttribute(VertexAttribute("normal", 3, 0, 3 * sizeof(GLfloat) * m_vertexBuffer.getVertexCount()));

    VertexArray::release();
}

VertexArray* CubeMesh::getVertexArray()
{
    return &m_vertexArray;
}

}