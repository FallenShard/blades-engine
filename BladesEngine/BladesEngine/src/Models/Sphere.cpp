#include <vector>
#include <fstream>
#include <iostream>

#include "Renderer/VertexAssembly.h"
#include "Renderer/Technique.h"

#include "Models/Sphere.h"

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
    Sphere::Sphere(Technique* prog)
        : m_program(prog)
    {
        init();
    }

    Sphere::~Sphere()
    {
    }


    void Sphere::init()
    {
        float sliceIncr = 360.f / m_slices;
        float ringIncr = 180.f / (m_rings + 1);

        std::vector<GLfloat> m_vertices;
        std::vector<GLushort> m_indices;

        // Fill the buffer with vertices and faces

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
            m_indices.push_back(0);
            m_indices.push_back(v);
        }

        m_indices.push_back(1);
        m_indices.push_back(0);
        m_indices.push_back(v);

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
                m_indices.push_back(v);
                m_indices.push_back(prevStart + curr + 1);

                m_indices.push_back(v + 1);
                m_indices.push_back(prevStart + curr + 1);
                m_indices.push_back(v);
                v++;
            }

            m_indices.push_back(prevStart);
            m_indices.push_back(prevEnd);
            m_indices.push_back(v);

            m_indices.push_back(prevEnd + 1);
            m_indices.push_back(prevStart);
            m_indices.push_back(v);

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
            m_indices.push_back(vertexcount - 1);
            m_indices.push_back(v);
        }
        m_indices.push_back(vertexcount - 1 - m_slices);
        m_indices.push_back(vertexcount - 2);
        m_indices.push_back(vertexcount - 1);

        glCreateVertexArrays(1, &m_vao);
        glCreateBuffers(1, &m_vbo);
        glCreateBuffers(1, &m_ibo);

        glNamedBufferData(m_vbo, m_vertices.size() * sizeof(GLfloat), m_vertices.data(), GL_STATIC_DRAW);
        glNamedBufferData(m_ibo, m_indices.size() * sizeof(GLushort), m_indices.data(), GL_STATIC_DRAW);

        glVertexArrayVertexBuffer(m_vao, VertexBufferBinding::Slot0, m_vbo, 0, 6 * sizeof(GLfloat));
        glVertexArrayElementBuffer(m_vao, m_ibo);

        glVertexArrayAttribBinding(m_vao, VertexAttrib::Position, VertexBufferBinding::Slot0);
        glVertexArrayAttribFormat(m_vao, VertexAttrib::Position, 3, GL_FLOAT, GL_FALSE, 0);
        glEnableVertexArrayAttrib(m_vao, VertexAttrib::Position);

        glVertexArrayAttribBinding(m_vao, VertexAttrib::Normal, VertexBufferBinding::Slot0);
        glVertexArrayAttribFormat(m_vao, VertexAttrib::Normal, 3, GL_FLOAT, GL_FALSE, 0);
        glEnableVertexArrayAttrib(m_vao, VertexAttrib::Normal);

        indCount = m_indices.size();

        std::vector<GLfloat> uboData;
        uboData.push_back(1.f); uboData.push_back(0.f); uboData.push_back(0.f); uboData.push_back(1.f);
        uboData.push_back(1.f); uboData.push_back(0.f); uboData.push_back(0.f); uboData.push_back(1.f);
        uboData.push_back(1.f); uboData.push_back(1.f); uboData.push_back(1.f); uboData.push_back(1.f);
        uboData.push_back(64.f);

        GLuint prog = 0;// = m_program->getProgramId();

        static GLuint UBO_BINDING = 0;

        glUseProgram(prog);
        std::string blockName = "Material";
        GLuint blockIndex = glGetUniformBlockIndex(prog, blockName.c_str());
        glUniformBlockBinding(prog, blockIndex, UBO_BINDING);

        glCreateBuffers(1, &m_ubo);
        glNamedBufferData(m_ubo, uboData.size() * sizeof(GLfloat), uboData.data(), GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, UBO_BINDING, m_ubo);
    }

    void Sphere::update(const float deltaTime)
    {
        timeVal += deltaTime;
    }

    void Sphere::render(const glm::mat4& projection, const glm::mat4& view)
    {
        glm::mat4 model = glm::translate(glm::vec3(0.f, 0.f, 0.f));

        //GLuint progId;// = m_program->getProgramId();
        m_program->use();

        glm::mat4 MVP = projection * view * model;
        m_program->setUniformAttribute("MVP", MVP);

        glm::mat4 MV = view * model;
        m_program->setUniformAttribute("MV", MV);

        glm::mat3 normalMatrix = glm::mat3(glm::inverseTranspose(MV));
        m_program->setUniformAttribute("normalMatrix", normalMatrix);

        glm::vec4 mvLightPos = view * glm::vec4(2.f, 2.f, 2.f, 1.f);
        m_program->setUniformAttribute("mvLightPos", mvLightPos);

        glBindVertexArray(m_vao);
        glDrawElements(GL_TRIANGLES, indCount, GL_UNSIGNED_SHORT, 0);
    }
}