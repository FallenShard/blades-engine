#include "Models/Sphere.h"

Sphere::Sphere()
    : m_radius(5.f)
    , m_rings(10)
    , m_slices(20)
    , m_center(0.f, 0.f, 0.f)
    , m_buffer(GL_STATIC_DRAW)
{
    init();
}

Sphere::Sphere(float radius, int rings, int slices, ShaderProgram* program)
    : SceneNode(nullptr, program)
    , m_radius(radius)
    , m_rings(rings)
    , m_slices(slices)
    , m_center(0.f, 0.f, 0.f)
    , m_buffer(GL_STATIC_DRAW)
{
    init();
}

Sphere::~Sphere()
{
    delete m_vertexArray;
}

void Sphere::render()
{
    m_shaderProgram->use();
    m_shaderProgram->setUniformAttribute("modelToWorldMatrix", m_absoluteTrans);
    m_shaderProgram->setUniformAttribute("useLight", 2.f);
    m_vertexArray->bind();

    m_vertexArray->setPrimitiveType(GL_TRIANGLES);
    m_vertexArray->renderIndexed();

    m_shaderProgram->setUniformAttribute("useLight", -2.f);
    glLineWidth(2.f);
    m_vertexArray->setPrimitiveType(GL_LINE_LOOP);
    //m_vertexArray->renderIndexed();

    //m_vertexArray->setPrimitiveType(GL_LINE_STRIP);
    //m_vertexArray->render();

    
}

void Sphere::setPrimitiveType(GLenum primType)
{
    m_vertexArray->setPrimitiveType(primType);
}

void Sphere::init()
{
    // Create new vertex array for this sphere
    m_vertexArray = new VertexArray();
    m_vertexArray->bind();
    
    float sliceIncr = 360.f / m_slices;
    float ringIncr = 180.f / (m_rings + 1);

    // Fill the buffer with vertices and faces
    m_buffer.setDataCountPerVertex(7);

    m_buffer.push(0.f, m_radius, 0.f);
    m_buffer.push(0.f, 1.f, 0.f, 1.f);

    // Faces for initial sphere fan cap
    short int v = 1;
    for (v = 1; v < m_slices; v++)
    {
        m_indexBuffer.push(v + 1, v, 0);
    }
    m_indexBuffer.push(1, v, 0);
    
    v = m_slices + 1;
    int x = 1;
    for (int ring = 1; ring <= m_rings; ring++)
    {
        float yCos = cos(glm::radians(ring * ringIncr));
        float ySin = sin(glm::radians(ring * ringIncr));

        for (int slice = m_slices; slice > 0; slice--)
        {
            float rads = glm::radians(slice * sliceIncr);
            m_buffer.push(m_radius * ySin * cos(rads), m_radius * yCos, m_radius * ySin * sin(rads));
            m_buffer.push(0.f, 1.f, 0.f, 1.f);
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

    m_buffer.push(0.f, -m_radius, 0.f);
    m_buffer.push(0.f, 1.f, 0.f, 1.f);

    int vertexCount = m_buffer.getVertexCount();

    // Faces for lower sphere fan cap
    for (v = vertexCount - m_slices; v < vertexCount - 1; v++)
    {
        m_indexBuffer.push(v - 1, v, vertexCount - 1);
    }
    m_indexBuffer.push(vertexCount - 1 - m_slices, vertexCount - 1, vertexCount - 2);

    // Bind the buffers into the vertex array
    m_buffer.bind();
    m_buffer.uploadData();
    m_vertexArray->setVertexCount(vertexCount);

    m_indexBuffer.bind();
    m_indexBuffer.uploadData();
    m_vertexArray->attachIndexBuffer(&m_indexBuffer);

    // Attach attributes
    m_vertexArray->attachAttribute(VertexAttribute("vPosition", 3, 7 * sizeof(GLfloat), 0));
    m_vertexArray->attachAttribute(VertexAttribute("vColor"   , 4, 7 * sizeof(GLfloat), 3 * sizeof(GLfloat)));
    m_vertexArray->enableAttributes(m_shaderProgram->getProgramId());

    m_shaderProgram->getUniformAttribute("useLight");
    VertexArray::release();
}