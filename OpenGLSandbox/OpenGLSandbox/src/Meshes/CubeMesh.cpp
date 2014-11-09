#include "Meshes/CubeMesh.h"
#include "Utils/VertexLoader.h"

PrismMesh::PrismMesh()
    : m_vertexBuffer(GL_STATIC_DRAW)
{
    init();
}

PrismMesh::~PrismMesh()
{
}

void PrismMesh::init()
{
    m_vertexArray.bind();

    m_vertexBuffer.bind();
    VertexLoader vLoader;
    vLoader.loadFromFile("res/CubeMesh.txt", m_vertexBuffer);
    m_vertexArray.setVertexCount(m_vertexBuffer.getVertexCount());

    m_indexBuffer.bind();
    m_indexBuffer.loadFromFile("res/RobotArmFaces.txt");
    m_vertexArray.attachIndexBuffer(&m_indexBuffer);

    m_vertexArray.attachAttribute(VertexAttribute("vPosition", 3, 0, 0));

    VertexArray::release();
}

VertexArray* PrismMesh::getVertexArray()
{
    return &m_vertexArray;
}

