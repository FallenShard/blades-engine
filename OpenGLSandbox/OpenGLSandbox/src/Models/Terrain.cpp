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
    GLint heightMap;

    GLint fineMap;
    GLuint fineTex;
    GLuint fineSampler;

    GLint detailMap;
    GLuint dirtTex;
    GLuint dirtSampler;

    GLint noiseMap;
    GLint detMap;
    GLuint noiseTex;
    GLuint detTex;

    GLint heightTexUnit = 0;
    GLint fineTexUnit = 2;
    GLint dirtTexUnit = 3;
    GLint noiseTexUnit = 11;
    GLint detTexUnit = 12;

    GLuint timeUnif;
    GLfloat timeVal = 0.f;

    int m_tiles = 128;
    float m_tileSize = 1.f;
    float m_worldScale = 24.f;
    float m_heightFactor = m_tiles / 16.f;

    float m_terrainSize = m_tiles * m_tileSize * m_worldScale;

    int vertCount = 0;
    int indCount = 0;

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
    : m_wireframe(0)
    , m_triSize(8.f)
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
    for (int z = 0; z < m_tiles + 1; z++)
    {
        for (int x = 0; x < m_tiles + 1; x++)
        {
            m_vertices.push_back(x * m_tileSize - m_tiles * m_tileSize / 2.f);
            m_vertices.push_back(0.f);
            m_vertices.push_back(m_tiles * m_tileSize / 2.f - z * m_tileSize);
        }
    }

    for (int z = 0; z < m_tiles; z++)
    {
        for (int x = 0; x < m_tiles; x++)
        {
            m_indices.push_back(z * (m_tiles + 1) + x);
            m_indices.push_back(z * (m_tiles + 1) + x + 1);
            m_indices.push_back((z + 1) * (m_tiles + 1) + x + 1);
            m_indices.push_back((z + 1) * (m_tiles + 1) + x);
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

    int nw, nh, ncomp;
    unsigned char* nimg = stbi_load("res/hmap.png", &nw, &nh, &ncomp, STBI_rgb_alpha);

    int detw, deth, detcomp;
    unsigned char* detimg = stbi_load("res/seamlessmoon.png", &detw, &deth, &detcomp, STBI_rgb_alpha);


    m_program->use();

    
    heightMap = glGetUniformLocation(m_program->getProgramId(), "hMap");
    fineMap = glGetUniformLocation(m_program->getProgramId(), "fMap");
    detailMap = glGetUniformLocation(m_program->getProgramId(), "dMap");
    noiseMap = glGetUniformLocation(m_program->getProgramId(), "nMap");
    detMap = glGetUniformLocation(m_program->getProgramId(), "detMap");

    m_program->setUniformSampler(heightMap, heightTexUnit);
    m_program->setUniformSampler(fineMap, fineTexUnit);
    m_program->setUniformSampler(detailMap, dirtTexUnit);
    m_program->setUniformSampler(noiseMap, noiseTexUnit);
    m_program->setUniformSampler(detMap, detTexUnit);

    glCreateTextures(GL_TEXTURE_2D, 1, &m_tex);
    glTextureStorage2D(m_tex, 1, GL_RGBA8, w, h);
    glTextureSubImage2D(m_tex, 0, 0, 0, w, h, GL_BGRA, GL_UNSIGNED_BYTE, img);
    glGenerateTextureMipmap(m_tex);

    glCreateTextures(GL_TEXTURE_2D, 1, &fineTex);
    glTextureStorage2D(fineTex, 1, GL_RGBA8, fw, fh);
    glTextureSubImage2D(fineTex, 0, 0, 0, fw, fh, GL_RGBA, GL_UNSIGNED_BYTE, fimg);
    glGenerateTextureMipmap(fineTex);

    glCreateTextures(GL_TEXTURE_2D, 1, &dirtTex);
    glTextureStorage2D(dirtTex, 8, GL_RGBA8, dw, dh);
    glTextureSubImage2D(dirtTex, 0, 0, 0, dw, dh, GL_RGBA, GL_UNSIGNED_BYTE, dimg);
    glGenerateTextureMipmap(dirtTex);

    glCreateTextures(GL_TEXTURE_2D, 1, &noiseTex);
    glTextureStorage2D(noiseTex, 8, GL_RGBA8, nw, nh);
    glTextureSubImage2D(noiseTex, 0, 0, 0, nw, nh, GL_RGBA, GL_UNSIGNED_BYTE, nimg);
    glGenerateTextureMipmap(noiseTex);

    glCreateTextures(GL_TEXTURE_2D, 1, &detTex);
    glTextureStorage2D(detTex, 8, GL_RGBA8, detw, deth);
    glTextureSubImage2D(detTex, 0, 0, 0, detw, deth, GL_RGBA, GL_UNSIGNED_BYTE, detimg);
    glGenerateTextureMipmap(detTex);

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

    glBindTextureUnit(noiseTexUnit, noiseTex);
    glBindSampler(noiseTexUnit, dirtSampler);

    glBindTextureUnit(detTexUnit, detTex);
    glBindSampler(detTexUnit, dirtSampler);

    //m_program->setUniformAttribute("patchSize", static_cast<float>(m_tileSize));
    m_program->setUniformAttribute("patches", static_cast<float>(m_tiles));
    m_program->setUniformAttribute("terrainSize", static_cast<float>(m_terrainSize));
    m_program->setUniformAttribute("worldScale", m_worldScale);
    m_program->setUniformAttribute("heightFactor", m_heightFactor);

    m_program->setUniformAttribute("triSize", m_triSize);
    m_program->setUniformAttribute("screenSize", glm::vec2(1366.f, 768.f));
    m_program->setUniformAttribute("detailFactor", 0.075f);

    stbi_image_free(img);
    stbi_image_free(fimg);
    stbi_image_free(dimg);

    glLineWidth(1);

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
    glm::mat4 model = glm::translate(glm::vec3(0.f, -m_worldScale * 10.f, 0.f)) * glm::scale(glm::vec3(m_worldScale));
    glm::mat4 mat = projection * view * model;
    glm::mat4 mv = view * model;
    glm::mat4 proj(projection);

    glm::mat4 viewInverse = glm::inverse(view);
    glm::vec4 cameraPos = viewInverse[3];
    glm::vec4 viewDir = -viewInverse[2];

    
    m_program->use();
    m_program->setUniformAttribute("MVP", mat);
    m_program->setUniformAttribute("MV", mv);
    m_program->setUniformAttribute("P", proj);

    //m_program->setUniformAttribute("cameraPos", cameraPos);
    //m_program->setUniformAttribute("viewDir", viewDir);
    m_program->setUniformAttribute("mvLightDir", glm::normalize(view * glm::normalize(glm::vec4(1.f, 1.f, 0.f, 0.f))));

    m_program->setUniformAttribute("wireframe", m_wireframe);
    glm::mat3 normalMatrix = glm::mat3(glm::inverseTranspose(mv));
    m_program->setUniformAttribute("normalMatrix", normalMatrix);

    glBindVertexArray(m_vao);

    if (m_wireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_PATCHES, indCount, GL_UNSIGNED_SHORT, 0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    else
    {
        glDrawElements(GL_PATCHES, indCount, GL_UNSIGNED_SHORT, 0);
    }
   
    //m_program->setUniformAttribute("mvLightDir", view * glm::vec4(0.f, -1.f, 0.f, 0.f));

    lineProgram->use();
    glm::mat4 lModel(1.f);
    glm::mat4 lineMat = projection * view * lModel;
    lineProgram->setUniformAttribute("MVP", lineMat);

    glBindVertexArray(lineVao);
    //glDrawArrays(GL_LINES, 0, 6);
}

void Terrain::setWireframe(bool enabled)
{
    m_wireframe = enabled;
}

void Terrain::setTriSize(int triSize)
{
    m_triSize = static_cast<float>(triSize);
    m_program->use();
    m_program->setUniformAttribute("triSize", m_triSize);
}

void Terrain::resizeScreenSpace(int width, int height)
{
    m_program->setUniformAttribute("screenSize", glm::vec2(width, height));
}

}