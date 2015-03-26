#include "Models/NewSphere.h"
#include <fstream>
#include <iostream>


#include "OglWrapper/IndexBuffer.h"
#include "OglWrapper/VertexBuffer.h"
#include "OglWrapper/VertexStreams.h"
#include "OglWrapper/ShaderProgram.h"

#include "Renderer/RenderComponent.h"

namespace
{
    GLuint texId;
    GLint hMap;

    GLuint timeUnif;
    GLfloat timeVal = 0.f;

    GLuint tbo;

    int m_slices = 32;
    int m_rings = 16;
    float m_radius = 1.f;
    

    int vertCount = 0;
    int indCount = 0;

}

namespace fsi
{

NewSphere::NewSphere()
{
}

NewSphere::NewSphere(PlaneMesh* mesh, PhongMaterial* material, ShaderProgram* prog)
    //: m_renderComp(std::make_unique<RenderComponent>(mesh, material))
    : //m_mesh(mesh)
    m_material(material)
    , m_program(prog)
    //, m_vertexArray(GL_PATCHES)
{
    //m_renderComp->setPrimitiveType(GL_PATCHES);
}

NewSphere::~NewSphere()
{
}


void NewSphere::init()
{
    float sliceIncr = 360.f / m_slices;
    float ringIncr = 180.f / (m_rings + 1);

    std::vector<GLfloat> m_vertices;
    std::vector<GLushort> m_indices;

    // Fill the buffer with vertices and faces
    //m_vertexBuffer.setVertexSize(6);

    m_vertices.push_back(0.f);      //vert
    m_vertices.push_back(m_radius);
    m_vertices.push_back(0.f);
    m_vertices.push_back(0.f);       //normal
    m_vertices.push_back(m_radius);
    m_vertices.push_back(0.f);

    // Faces for initial sphere fan cap
    unsigned short v = 1;
    for (v = 1; v < m_slices; v++)
    {
        m_indices.push_back(v + 1);
        m_indices.push_back(v);
        m_indices.push_back(0);
    }

    m_indices.push_back(1);
    m_indices.push_back(v);
    m_indices.push_back(0);

    v = m_slices + 1;

    // Intermediate vertices and faces
    for (int ring = 1; ring <= m_rings; ring++)
    {
        float yCos = cos(glm::radians(ring * ringIncr));
        float ySin = sin(glm::radians(ring * ringIncr));

        for (int slice = m_slices; slice > 0; slice--)
        {
            float rads = glm::radians(slice * sliceIncr);
            m_vertices.push_back(m_radius * ySin * cos(rads));
            m_vertices.push_back(m_radius * yCos);
            m_vertices.push_back(m_radius * ySin * sin(rads));
                
            m_vertices.push_back(m_radius * ySin * cos(rads));
            m_vertices.push_back(m_radius * yCos);
            m_vertices.push_back(m_radius * ySin * sin(rads));
        }

        if (ring == m_rings)
            break;
        short int prevStart = v - m_slices;
        short int prevEnd = v - 1;
        short int curr;
        for (curr = 0; curr < m_slices - 1; curr++)
        {
            m_indices.push_back(prevStart + curr);
            m_indices.push_back(prevStart + curr + 1);
            m_indices.push_back(v);

            m_indices.push_back(v + 1);
            m_indices.push_back(v);
            m_indices.push_back(prevStart + curr + 1);
            v++;
        }

        m_indices.push_back(prevStart);
        m_indices.push_back(v);
        m_indices.push_back(prevEnd);

        m_indices.push_back(prevEnd + 1);
        m_indices.push_back(v);
        m_indices.push_back(prevStart);

        v++;
    }

    m_vertices.push_back(0.f);
    m_vertices.push_back(-m_radius);
    m_vertices.push_back(0.f);
    m_vertices.push_back(0.f);
    m_vertices.push_back(-m_radius);
    m_vertices.push_back(0.f);

    int vertexcount = m_vertices.size() / 6;

    // faces for lower sphere fan cap
    for (v = vertexcount - m_slices; v < vertexcount - 1; v++)
    {
        m_indices.push_back(v - 1);
        m_indices.push_back(v);
        m_indices.push_back(vertexcount - 1);
    }
    m_indices.push_back(vertexcount - 1 - m_slices);
    m_indices.push_back(vertexcount - 1);
    m_indices.push_back(vertexcount - 2);

    glCreateVertexArrays(1, &m_vao);
    glCreateBuffers(1, &m_vbo);
    glCreateBuffers(1, &m_ibo);

    glNamedBufferData(m_vbo, m_vertices.size() * sizeof(GLfloat), m_vertices.data(), GL_STATIC_DRAW);
    glNamedBufferData(m_ibo, m_indices.size() * sizeof(GLushort), m_indices.data(), GL_STATIC_DRAW);

    glVertexArrayVertexBuffer(m_vao, VertexBufferBinding::Position, m_vbo, 0, 6 * sizeof(GLfloat));
    glVertexArrayElementBuffer(m_vao, m_ibo);

    glVertexArrayAttribBinding(m_vao, VertexAttrib::Position, VertexBufferBinding::Position);
    glVertexArrayAttribFormat(m_vao, VertexAttrib::Position, 3, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(m_vao, VertexAttrib::Position);

    glVertexArrayAttribBinding(m_vao, VertexAttrib::Normal, VertexBufferBinding::Position);
    glVertexArrayAttribFormat(m_vao, VertexAttrib::Normal, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat));
    glEnableVertexArrayAttrib(m_vao, VertexAttrib::Normal);

    indCount = m_indices.size();

    std::vector<GLfloat> uboData;
    uboData.push_back(1.f); uboData.push_back(0.f); uboData.push_back(0.f); uboData.push_back(1.f);
    uboData.push_back(1.f); uboData.push_back(0.f); uboData.push_back(0.f); uboData.push_back(1.f);
    uboData.push_back(1.f); uboData.push_back(1.f); uboData.push_back(1.f); uboData.push_back(1.f);
    uboData.push_back(64.f);

    GLuint prog = m_program->getProgramId();

    static GLuint UBO_BINDING = 0;

    glUseProgram(prog);
    std::string blockName = "Material";
    GLuint blockIndex = glGetUniformBlockIndex(prog, blockName.c_str());
    glUniformBlockBinding(prog, blockIndex, UBO_BINDING);

    glGenBuffers(1, &m_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
    //glCreateBuffers(1, &m_ubo);

    
    glBufferData(GL_UNIFORM_BUFFER, uboData.size() * sizeof(GLfloat), uboData.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, UBO_BINDING, m_ubo);

    //m_uniformBuffer = new UniformBuffer();
    //m_uniformBuffer->bind();
    //m_uniformBuffer->setBufferData(&m_data, sizeof(PhongData));
    
    

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    //m_program->setUniformBlockBinding(blockName, 0);
    
}

void NewSphere::update(const float deltaTime)
{
    timeVal += deltaTime;
}

void NewSphere::render(const glm::mat4& projection, const glm::mat4& view)
{
    GLenum err = glGetError();
    glm::mat4 model = glm::translate(glm::vec3(0.f, 0.f, 0.f));
    
    GLuint progId = m_program->getProgramId();
    m_program->use();
    //glUseProgram(progId);

    glm::mat4 MVP = projection * view * model;
    m_program->setUniformAttribute("MVP", MVP);
    //GLuint mvpPos = m_program->getUniformAttribute("MVP");
    //glProgramUniformMatrix4fv(progId, mvpPos, 1, GL_FALSE, glm::value_ptr(MVP));

    glm::mat4 MV = view * model;
    m_program->setUniformAttribute("MV", MV);
    //GLuint mvPos = m_program->getUniformAttribute("MV");
    //glProgramUniformMatrix4fv(progId, mvpPos, 1, GL_FALSE, glm::value_ptr(MV));

    glm::mat3 normalMatrix = glm::mat3(glm::inverseTranspose(MV));
    m_program->setUniformAttribute("normalMatrix", normalMatrix);
    //GLuint normPos = m_program->getUniformAttribute("normalMatrix");
    //glProgramUniformMatrix3fv(progId, normPos, 1, GL_FALSE, glm::value_ptr(normalMatrix));

    glm::vec4 mvLightPos = view * glm::vec4(2.f, 2.f, 2.f, 1.f);
    m_program->setUniformAttribute("mvLightPos", mvLightPos);


    //if (m_material) m_material->apply();

    
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, indCount, GL_UNSIGNED_SHORT, 0);
}

}