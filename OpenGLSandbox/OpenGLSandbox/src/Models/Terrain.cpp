#include "Models/Terrain.h"
#include <fstream>
#include <iostream>


#include "OglWrapper/IndexBuffer.h"
#include "OglWrapper/VertexBuffer.h"
#include "OglWrapper/VertexStreams.h"
#include "OglWrapper/ShaderProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "Renderer/RenderComponent.h"

namespace
{
    GLuint texId;
    GLint hMap;

    GLuint timeUnif;
    GLfloat timeVal = 0.f;

    GLuint tbo;

    int m_zSquares = 32;
    int m_xSquares = 32;
    float m_squareSize = 32.f;

    float m_xTotalSize = m_xSquares * m_squareSize;
    float m_zTotalSize = m_zSquares * m_squareSize;

    int vertCount = 0;
    int indCount = 0;


    GLuint prog;
    GLuint vert;
    GLuint control;
    GLuint eval;
    GLuint frag;

    std::vector<GLfloat> m_vertices;
    std::vector<GLushort> m_indices;
}



namespace fsi
{

Terrain::Terrain()
{
}

Terrain::Terrain(PlaneMesh* mesh, HeightMapMaterial* material, ShaderProgram* prog)
    //: m_renderComp(std::make_unique<RenderComponent>(mesh, material))
    : //m_mesh(mesh)
     m_material(material)
    , m_program(prog)
    //, m_vertexArray(GL_PATCHES)
{
    //m_renderComp->setPrimitiveType(GL_PATCHES);
}

Terrain::~Terrain()
{
}


void Terrain::init()
{
    for (int z = 0; z < m_zSquares + 1; z++)
    {
        for (int x = 0; x < m_xSquares + 1; x++)
        {
            m_vertices.push_back(x * m_squareSize - m_xTotalSize / 2.f);
            m_vertices.push_back(0.f);
            m_vertices.push_back(z * m_squareSize - m_zTotalSize / 2.f);
        }
    }

    for (int z = 0; z < m_zSquares; z++)
    {
        for (int x = 0; x < m_xSquares; x++)
        {
            m_indices.push_back(z * (m_xSquares + 1) + x);
            m_indices.push_back(z * (m_xSquares + 1) + x + 1);
            m_indices.push_back((z + 1) * (m_xSquares + 1) + x + 1);
            m_indices.push_back((z + 1) * (m_xSquares + 1) + x);
        }
    }

    
    glCreateVertexArrays(1, &m_vao);
    glCreateBuffers(1, &m_vbo);
    glCreateBuffers(1, &m_ibo);

    glNamedBufferData(m_vbo, m_vertices.size() * sizeof(GLfloat), m_vertices.data(), GL_STATIC_DRAW);
    glNamedBufferData(m_ibo, m_indices.size() * sizeof(GLushort), m_indices.data(), GL_STATIC_DRAW);
    
    glVertexArrayVertexBuffer(m_vao, VertexBufferBinding::Position, m_vbo, 0, 3 * sizeof(GLfloat));
    glVertexArrayElementBuffer(m_vao, m_ibo);

    glVertexArrayAttribBinding(m_vao, VertexAttrib::Position, VertexBufferBinding::Position);
    glVertexArrayAttribFormat(m_vao, VertexAttrib::Position, 3, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(m_vao, VertexAttrib::Position);
    
    glPatchParameteri(GL_PATCH_VERTICES, 4);
    /*
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ibo);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * 4, m_vertices.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLushort), m_indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, (const void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);*/


    vertCount = m_vertices.size() / 3;

    indCount = m_indices.size();

    /*m_vertexArray.bind();

    m_mesh->bind();

    const std::vector<VertexAttribute>& attribs = m_mesh->getVertexAttributes();

    for (auto attribute : attribs)
    {
        m_vertexArray.enableVertexAttrib(attribute.attribLoc);
        m_vertexArray.attributeFormat(attribute);
        m_vertexArray.attributeBinding(attribute.attribLoc, RenderComponent::bindingTable[attribute.attribLoc]);
    }

    m_vertexArray.setVertexCount(m_mesh->getVertexCount());
    m_vertexArray.setIndexCount(m_mesh->getIndexCount());*/

    //m_material->init();

    int w;
    int h;
    int comp;

    m_program->use();

    //m_program->setUniformSampler(hMap)
    //glUniform1i(hMap, 1);

    hMap = glGetUniformLocation(m_program->getProgramId(), "hMap");

    
    

    unsigned char* img = stbi_load("res/heightmap.png", &w, &h, &comp, STBI_rgb_alpha);

    //glGenTextures(1, &texId);
    glCreateTextures(GL_TEXTURE_2D, 1, &m_tex);
    glTextureStorage2D(m_tex, 1, GL_RGBA8, w, h);
    glTextureSubImage2D(m_tex, 0, 0, 0, w, h, GL_BGRA, GL_UNSIGNED_BYTE, img);
    glGenerateTextureMipmap(m_tex);

    glCreateSamplers(1, &m_sampler);
    glSamplerParameteri(m_sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(m_sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glBindTextureUnit(0, m_tex);
    glBindSampler(0, m_sampler);
    m_program->setUniformSampler(hMap, 0);
    m_program->setUniformAttribute("patchSize", (float)m_squareSize);
    m_program->setUniformAttribute("patchesX", (float)m_xSquares);

    stbi_image_free(img);

    glLineWidth(3);
}

void Terrain::update(const float deltaTime)
{
    timeVal += deltaTime;
}

void Terrain::render(const glm::mat4& projection, const glm::mat4& view)
{
    glm::mat4 model = glm::translate(glm::vec3(0.f, -50.f, 0.f));
    glm::mat4 mat = projection * view * model;
    glm::mat4 mv = view * model;
    glm::mat4 proj(projection);

    glm::vec4 cameraPos = glm::inverse(view)[3];

    
    m_program->use();
    m_program->setUniformAttribute("MVP", mat);
    m_program->setUniformAttribute("MV", mv);
    m_program->setUniformAttribute("P", proj);

    //m_program->setUniformAttribute("cameraPos", cameraPos);
    //m_program->setUniformAttribute("mvLightDir", view * glm::vec4(0.f, -1.f, 0.f, 0.f));
    
    glBindVertexArray(m_vao);

    m_program->setUniformAttribute("wireframe", 1);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_PATCHES, indCount, GL_UNSIGNED_SHORT, 0);

    model = glm::translate(glm::vec3(0.f, -50.01f, 0.f));
    mat = projection * view * model;
    mv = view * model;
    m_program->setUniformAttribute("MVP", mat);
    m_program->setUniformAttribute("MV", mv);
    m_program->setUniformAttribute("wireframe", 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawElements(GL_PATCHES, indCount, GL_UNSIGNED_SHORT, 0);
}



}