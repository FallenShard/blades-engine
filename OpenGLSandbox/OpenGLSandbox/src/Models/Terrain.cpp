#include <fstream>
#include <iostream>

#include "OglWrapper/VertexStreams.h"
#include "OglWrapper/ShaderProgram.h"
#include "Renderer/ShaderManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "Models/Terrain.h"

namespace
{
    GLuint texId;
    GLint hMap;
    GLint fMap;
    GLint dMap;

    GLuint timeUnif;
    GLfloat timeVal = 0.f;

    GLuint tbo;

    int m_zSquares = 64;
    int m_xSquares = 64;
    float m_squareSize = 8.f;

    float m_xTotalSize = m_xSquares * m_squareSize;
    float m_zTotalSize = m_zSquares * m_squareSize;

    int vertCount = 0;
    int indCount = 0;

    GLuint prog;
    GLuint vert;
    GLuint control;
    GLuint eval;
    GLuint frag;

    GLuint fineTex;
    GLuint fineSampler;
    GLuint dirtTex;
    GLuint dirtSampler;

    std::vector<GLfloat> m_vertices;
    std::vector<GLushort> m_indices;

    GLuint lineVao;
    GLuint lineVbo;
    std::vector<GLfloat> lineVerts;
    
    fsi::ShaderProgram* lineProgram;

    void fillOriginBuffer()
    {
        // X
        lineVerts.push_back(0.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(1.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(1.f);
        lineVerts.push_back(10.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(1.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(1.f);

        // Y
        lineVerts.push_back(0.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(1.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(1.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(10.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(1.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(1.f);

        // Z
        lineVerts.push_back(0.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(1.f);
        lineVerts.push_back(1.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(10.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(1.f);
        lineVerts.push_back(1.f);
    }
}

namespace fsi
{

Terrain::Terrain(ShaderManager* manager)
{
    m_program = manager->getProgram("terrain");
    lineProgram = manager->getProgram("color");

    init();
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
            m_vertices.push_back(m_zTotalSize / 2.f - z * m_squareSize);
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
    
    vertCount = m_vertices.size() / 3;

    indCount = m_indices.size();

    glCreateVertexArrays(1, &m_vao);
    glCreateBuffers(1, &m_vbo);
    glCreateBuffers(1, &m_ibo);

    glNamedBufferData(m_vbo, m_vertices.size() * sizeof(GLfloat), m_vertices.data(), GL_STATIC_DRAW);
    glNamedBufferData(m_ibo, m_indices.size() * sizeof(GLushort), m_indices.data(), GL_STATIC_DRAW);
    
    glVertexArrayVertexBuffer(m_vao, VertexBufferBinding::Slot0, m_vbo, 0, 3 * sizeof(GLfloat));
    glVertexArrayElementBuffer(m_vao, m_ibo);

    glVertexArrayAttribBinding(m_vao, VertexAttrib::Position, VertexBufferBinding::Slot0);
    glVertexArrayAttribFormat(m_vao, VertexAttrib::Position, 3, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(m_vao, VertexAttrib::Position);
    
    glPatchParameteri(GL_PATCH_VERTICES, 4);

    int w, h, comp;
    unsigned char* img = stbi_load("res/heightmap1024.png", &w, &h, &comp, STBI_rgb_alpha);

    int fw, fh, fcomp;
    unsigned char* fimg = stbi_load("res/noise.png", &fw, &fh, &fcomp, STBI_rgb_alpha);

    int dw, dh, dcomp;
    unsigned char* dimg = stbi_load("res/lunar.png", &dw, &dh, &dcomp, STBI_rgb_alpha);

    m_program->use();

    GLint heightTexUnit = 0;
    GLint fineTexUnit = 2;
    GLint dirtTexUnit = 3;
    hMap = glGetUniformLocation(m_program->getProgramId(), "hMap");
    fMap = glGetUniformLocation(m_program->getProgramId(), "fMap");
    dMap = glGetUniformLocation(m_program->getProgramId(), "dMap");

    m_program->setUniformSampler(hMap, heightTexUnit);
    m_program->setUniformSampler(fMap, fineTexUnit);
    m_program->setUniformSampler(dMap, dirtTexUnit);

    glCreateTextures(GL_TEXTURE_2D, 1, &m_tex);
    glTextureStorage2D(m_tex, 1, GL_RGBA8, w, h);
    glTextureSubImage2D(m_tex, 0, 0, 0, w, h, GL_BGRA, GL_UNSIGNED_BYTE, img);
    glGenerateTextureMipmap(m_tex);

    glCreateTextures(GL_TEXTURE_2D, 1, &fineTex);
    glTextureStorage2D(fineTex, 1, GL_RGBA8, fw, fh);
    glTextureSubImage2D(fineTex, 0, 0, 0, fw, fh, GL_BGRA, GL_UNSIGNED_BYTE, fimg);
    glGenerateTextureMipmap(fineTex);

    glCreateTextures(GL_TEXTURE_2D, 1, &dirtTex);
    glTextureStorage2D(dirtTex, 8, GL_RGBA8, dw, dh);
    glTextureSubImage2D(dirtTex, 0, 0, 0, dw, dh, GL_RGBA, GL_UNSIGNED_BYTE, dimg);
    glGenerateTextureMipmap(dirtTex);

    glCreateSamplers(1, &m_sampler);
    glSamplerParameteri(m_sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(m_sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glCreateSamplers(1, &fineSampler);
    glSamplerParameteri(fineSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(fineSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glSamplerParameteri(fineSampler, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glSamplerParameteri(fineSampler, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glCreateSamplers(1, &dirtSampler);
    glSamplerParameteri(dirtSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(dirtSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glSamplerParameteri(dirtSampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glSamplerParameteri(dirtSampler, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTextureUnit(heightTexUnit, m_tex);
    glBindSampler(heightTexUnit, m_sampler);

    glBindTextureUnit(fineTexUnit, fineTex);
    glBindSampler(fineTexUnit, fineSampler);

    glBindTextureUnit(dirtTexUnit, dirtTex);
    glBindSampler(dirtTexUnit, dirtSampler);

    m_program->setUniformAttribute("patchSize", (float)m_squareSize);
    m_program->setUniformAttribute("patchesX", (float)m_xSquares);

    stbi_image_free(img);
    stbi_image_free(fimg);
    stbi_image_free(dimg);

    glLineWidth(3);

    // Coord system
    fillOriginBuffer();

    glCreateVertexArrays(1, &lineVao);
    glCreateBuffers(1, &lineVbo);

    glNamedBufferData(lineVbo, lineVerts.size() * sizeof(GLfloat), lineVerts.data(), GL_STATIC_DRAW);

    glVertexArrayVertexBuffer(lineVao, VertexBufferBinding::Slot0, lineVbo, 0, 7 * sizeof(GLfloat));

    glVertexArrayAttribBinding(lineVao, VertexAttrib::Position, VertexBufferBinding::Slot0);
    glVertexArrayAttribFormat(lineVao, VertexAttrib::Position, 3, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(lineVao, VertexAttrib::Position);

    glVertexArrayAttribBinding(lineVao, VertexAttrib::Color, VertexBufferBinding::Slot0);
    glVertexArrayAttribFormat(lineVao, VertexAttrib::Color, 4, GL_FLOAT, GL_FALSE, 12);
    glEnableVertexArrayAttrib(lineVao, VertexAttrib::Color);
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
    //glDrawElements(GL_PATCHES, indCount, GL_UNSIGNED_SHORT, 0);

    model = glm::translate(glm::vec3(0.f, -50.01f, 0.f));
    mat = projection * view * model;
    mv = view * model;
    m_program->setUniformAttribute("MVP", mat);
    m_program->setUniformAttribute("MV", mv);
    m_program->setUniformAttribute("wireframe", 0);
    //m_program->setUniformAttribute("mvLightDir", view * glm::vec4(0.f, -1.f, 0.f, 0.f));
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawElements(GL_PATCHES, indCount, GL_UNSIGNED_SHORT, 0);

    lineProgram->use();
    glm::mat4 lModel(1.f);
    glm::mat4 lineMat = projection * view * lModel;
    lineProgram->setUniformAttribute("MVP", lineMat);

    glBindVertexArray(lineVao);
    glDrawArrays(GL_LINES, 0, 6);
   

    glEnable(GL_DEPTH_TEST);
}



}