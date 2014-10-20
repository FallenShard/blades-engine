#include "Models/PlaneGrid.h"

PlaneGrid::PlaneGrid(float size, float lineStep, int planes)
    : m_size(size)
    , m_lineStep(lineStep)
    , m_visiblePlanes(planes)
    , m_vertexBuffer(GL_STATIC_DRAW)
    , m_vertexArray(GL_LINES)
{
    buildVertices();


    m_vertexArray.attachAttribute(VertexAttribute("vPosition", 3, 7 * sizeof(GLfloat), 0));
    m_vertexArray.attachAttribute(VertexAttribute("vColor", 4, 7 * sizeof(GLfloat), 3 * sizeof(GLfloat)));
}

PlaneGrid::~PlaneGrid()
{
}

void PlaneGrid::setProgram(ShaderProgram* program)
{
    m_program = program;
    m_vertexArray.enableAttributes(m_program->getProgramId());
}

void PlaneGrid::displayPlanes(int planes)
{
    m_visiblePlanes = planes;
    buildVertices();

    if (m_program != nullptr)
        m_vertexArray.enableAttributes(m_program->getProgramId());
}

void PlaneGrid::render()
{
    m_vertexArray.bind();
    m_vertexArray.render();
}

void PlaneGrid::buildVertices()
{
    float halfSize = m_size / 2;

    m_vertexBuffer.clear();
    if ((m_visiblePlanes & XZ) != 0)
    {
        for (float z = -halfSize; z <= halfSize; z += m_lineStep)
        {
            m_vertexBuffer.push(-1.f * halfSize, 0.f, 1.f * z);
            m_vertexBuffer.push(0.6f, 0.f, 0.f, 1.f);

            m_vertexBuffer.push( 1.f * halfSize, 0.f, 1.f * z);
            m_vertexBuffer.push(0.6f, 0.f, 0.f, 1.f);
        }

        for (float x = -halfSize; x <= halfSize; x += m_lineStep)
        {
            m_vertexBuffer.push(1.f * x, 0.f, -1.f * halfSize);
            m_vertexBuffer.push(0.6f, 0.f, 0.f, 1.f);

            m_vertexBuffer.push(1.f * x, 0.f,  1.f * halfSize);
            m_vertexBuffer.push(0.6f, 0.f, 0.f, 1.f);
        }
    }

    if ((m_visiblePlanes & XY) != 0)
    {
        for (float x = -halfSize; x <= halfSize; x += m_lineStep)
        {
            m_vertexBuffer.push(1.f * x, -1.f * halfSize, 0.f);
            m_vertexBuffer.push(0.f, 0.6f, 0.f, 1.f);

            m_vertexBuffer.push(1.f * x,  1.f * halfSize, 0.f);
            m_vertexBuffer.push(0.f, 0.6f, 0.f, 1.f);
        }

        for (float y = -halfSize; y <= halfSize; y += m_lineStep)
        {
            m_vertexBuffer.push(-1.f * halfSize, 1.f * y, 0.f);
            m_vertexBuffer.push(0.f, 0.6f, 0.f, 1.f);

            m_vertexBuffer.push( 1.f * halfSize, 1.f * y, 0.f);
            m_vertexBuffer.push(0.f, 0.6f, 0.f, 1.f);
        }
    }

    if ((m_visiblePlanes & YZ) != 0)
    {
        for (float z = -halfSize; z <= halfSize; z += m_lineStep)
        {
            m_vertexBuffer.push(0.f, -1 * halfSize, 1.f * z);
            m_vertexBuffer.push(0.f, 0.f, 0.6f, 1.f);

            m_vertexBuffer.push(0.f,  1 * halfSize, 1.f * z);
            m_vertexBuffer.push(0.f, 0.f, 0.6f, 1.f);
        }

        for (float y = -halfSize; y <= halfSize; y += m_lineStep)
        {
            m_vertexBuffer.push(0.f, 1.f * y, -1.f * halfSize);
            m_vertexBuffer.push(0.f, 0.f, 0.6f, 1.f);

            m_vertexBuffer.push(0.f, 1.f * y,  1.f * halfSize);
            m_vertexBuffer.push(0.f, 0.f, 0.6f, 1.f);
        }
    }

    m_vertexArray.bind();

    m_vertexBuffer.bind();
    m_vertexBuffer.setDataCountPerVertex(7);
    m_vertexBuffer.uploadData();

    m_vertexArray.setVertexCount(m_vertexBuffer.getVertexCount());
}