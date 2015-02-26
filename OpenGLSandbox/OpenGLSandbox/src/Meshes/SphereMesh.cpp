#include "Meshes/SphereMesh.h"

namespace fsi
{

SphereMesh::SphereMesh()
    : m_radius(5.f)
    , m_rings(20)
    , m_slices(20)
    , m_vertexBuffer(GL_STATIC_DRAW)
{
    computeVertices();
}

SphereMesh::SphereMesh(float radius, short int rings, short int slices)
    : m_radius(radius)
    , m_rings(rings)
    , m_slices(slices)
    , m_vertexBuffer(GL_STATIC_DRAW)
{
    computeVertices();
}

SphereMesh::~SphereMesh()
{
}

void SphereMesh::computeVertices()
{
    float sliceIncr = 360.f / m_slices;
    float ringIncr = 180.f / (m_rings + 1);

    // Fill the buffer with vertices and faces
    m_vertexBuffer.setDataCountPerVertex(6);

    m_vertexBuffer.push(0.f, m_radius, 0.f);
    m_vertexBuffer.push(0.f, m_radius, 0.f);

    // Faces for initial sphere fan cap
    short int v = 1;
    for (v = 1; v < m_slices; v++)
    {
        m_indexBuffer.push(v + 1, v, 0);
    }
    m_indexBuffer.push(1, v, 0);

    v = m_slices + 1;
    
    // Intermediate vertices and faces
    for (int ring = 1; ring <= m_rings; ring++)
    {
        float yCos = cos(glm::radians(ring * ringIncr));
        float ySin = sin(glm::radians(ring * ringIncr));

        for (int slice = m_slices; slice > 0; slice--)
        {
            float rads = glm::radians(slice * sliceIncr);
            m_vertexBuffer.push(m_radius * ySin * cos(rads), m_radius * yCos, m_radius * ySin * sin(rads));
            m_vertexBuffer.push(m_radius * ySin * cos(rads), m_radius * yCos, m_radius * ySin * sin(rads));
        }

        if (ring == m_rings)
            break;
        short int prevStart = v - m_slices;
        short int prevEnd = v - 1;
        short int curr;
        for (curr = 0; curr < m_slices - 1; curr++)
        {
            m_indexBuffer.push(prevStart + curr, prevStart + curr + 1, v);
            m_indexBuffer.push(v + 1, v, prevStart + curr + 1);
            v++;
        }

        m_indexBuffer.push(prevStart, v, prevEnd);
        m_indexBuffer.push(prevEnd + 1, v, prevStart);

        v++;
    }

    m_vertexBuffer.push(0.f, -m_radius, 0.f);
    m_vertexBuffer.push(0.f, -m_radius, 0.f);

    int vertexcount = m_vertexBuffer.getVertexCount();

    // faces for lower sphere fan cap
    for (v = vertexcount - m_slices; v < vertexcount - 1; v++)
    {
        m_indexBuffer.push(v - 1, v, vertexcount - 1);
    }
    m_indexBuffer.push(vertexcount - 1 - m_slices, vertexcount - 1, vertexcount - 2);

    // Bind this VAO
    m_vertexArray.bind();

    // Bind the vertex buffer (binding happens later when enabling attributes)
    m_vertexBuffer.bind();
    m_vertexBuffer.uploadData();
    m_vertexArray.setVertexCount(m_vertexBuffer.getVertexCount());

    // Bind the index buffer (this becomes part of the VAO)
    m_indexBuffer.bind();
    m_indexBuffer.uploadData();
    m_vertexArray.attachIndexBuffer(&m_indexBuffer);

    m_vertexArray.attachAttribute(VertexAttribute("position", 3, 6 * sizeof(GLfloat), 0));
    m_vertexArray.attachAttribute(VertexAttribute("normal", 3, 6 * sizeof(GLfloat), 3 * sizeof(GLfloat)));
    
    VertexArray::release();
}

VertexArray* SphereMesh::getVertexArray()
{
    return &m_vertexArray;
}

}