#include <vector>
#include "Models/Skybox.h"
#include "OglWrapper/ShaderProgram.h"
#include "OglWrapper/VertexStreams.h"
#include "stb/stb_image.h"

namespace fsi
{

namespace
{
    GLfloat m_size = 5.f;
}

Skybox::Skybox(ShaderProgram* prog)
    : m_program(prog)
{
    std::vector<GLfloat> m_vertices;
    std::vector<GLushort> m_indices;

    // Front face
    m_vertices.push_back(-m_size);
    m_vertices.push_back(-m_size);
    m_vertices.push_back(-m_size);
    m_vertices.push_back(0.f);
    m_vertices.push_back(1.f);
    m_vertices.push_back(1.f);

    m_vertices.push_back(+m_size);
    m_vertices.push_back(-m_size);
    m_vertices.push_back(-m_size);
    m_vertices.push_back(1.f);
    m_vertices.push_back(1.f);
    m_vertices.push_back(1.f);

    m_vertices.push_back(+m_size);
    m_vertices.push_back(+m_size);
    m_vertices.push_back(-m_size);
    m_vertices.push_back(1.f);
    m_vertices.push_back(0.f);
    m_vertices.push_back(1.f);

    m_vertices.push_back(-m_size);
    m_vertices.push_back(+m_size);
    m_vertices.push_back(-m_size);
    m_vertices.push_back(0.f);
    m_vertices.push_back(0.f);
    m_vertices.push_back(1.f);

    m_indices.push_back(0);
    m_indices.push_back(1);
    m_indices.push_back(2);

    m_indices.push_back(0);
    m_indices.push_back(2);
    m_indices.push_back(3);

    // Right face
    m_vertices.push_back(+m_size);
    m_vertices.push_back(-m_size);
    m_vertices.push_back(-m_size);
    m_vertices.push_back(0.f);
    m_vertices.push_back(1.f);
    m_vertices.push_back(2.f);

    m_vertices.push_back(+m_size);
    m_vertices.push_back(-m_size);
    m_vertices.push_back(+m_size);
    m_vertices.push_back(1.f);
    m_vertices.push_back(1.f);
    m_vertices.push_back(2.f);

    m_vertices.push_back(+m_size);
    m_vertices.push_back(+m_size);
    m_vertices.push_back(+m_size);
    m_vertices.push_back(1.f);
    m_vertices.push_back(0.f);
    m_vertices.push_back(2.f);

    m_vertices.push_back(+m_size);
    m_vertices.push_back(+m_size);
    m_vertices.push_back(-m_size);
    m_vertices.push_back(0.f);
    m_vertices.push_back(0.f);
    m_vertices.push_back(2.f);

    m_indices.push_back(4);
    m_indices.push_back(5);
    m_indices.push_back(6);

    m_indices.push_back(4);
    m_indices.push_back(6);
    m_indices.push_back(7);

    // Left face
    m_vertices.push_back(-m_size);
    m_vertices.push_back(-m_size);
    m_vertices.push_back(+m_size);
    m_vertices.push_back(0.f);
    m_vertices.push_back(1.f);
    m_vertices.push_back(0.f);

    m_vertices.push_back(-m_size);
    m_vertices.push_back(-m_size);
    m_vertices.push_back(-m_size);
    m_vertices.push_back(1.f);
    m_vertices.push_back(1.f);
    m_vertices.push_back(0.f);

    m_vertices.push_back(-m_size);
    m_vertices.push_back(+m_size);
    m_vertices.push_back(-m_size);
    m_vertices.push_back(1.f);
    m_vertices.push_back(0.f);
    m_vertices.push_back(0.f);

    m_vertices.push_back(-m_size);
    m_vertices.push_back(+m_size);
    m_vertices.push_back(+m_size);
    m_vertices.push_back(0.f);
    m_vertices.push_back(0.f);
    m_vertices.push_back(0.f);

    m_indices.push_back(8);
    m_indices.push_back(9);
    m_indices.push_back(10);

    m_indices.push_back(8);
    m_indices.push_back(10);
    m_indices.push_back(11);

    // Rear face
    m_vertices.push_back(+m_size);
    m_vertices.push_back(-m_size);
    m_vertices.push_back(+m_size);
    m_vertices.push_back(0.f);
    m_vertices.push_back(1.f);
    m_vertices.push_back(3.f);

    m_vertices.push_back(-m_size);
    m_vertices.push_back(-m_size);
    m_vertices.push_back(+m_size);
    m_vertices.push_back(1.f);
    m_vertices.push_back(1.f);
    m_vertices.push_back(3.f);

    m_vertices.push_back(-m_size);
    m_vertices.push_back(+m_size);
    m_vertices.push_back(+m_size);
    m_vertices.push_back(1.f);
    m_vertices.push_back(0.f);
    m_vertices.push_back(3.f);

    m_vertices.push_back(+m_size);
    m_vertices.push_back(+m_size);
    m_vertices.push_back(+m_size);
    m_vertices.push_back(0.f);
    m_vertices.push_back(0.f);
    m_vertices.push_back(3.f);

    m_indices.push_back(12);
    m_indices.push_back(13);
    m_indices.push_back(14);

    m_indices.push_back(12);
    m_indices.push_back(14);
    m_indices.push_back(15);

    // Bottom face
    m_vertices.push_back(-m_size);
    m_vertices.push_back(-m_size);
    m_vertices.push_back(+m_size);
    m_vertices.push_back(0.f);
    m_vertices.push_back(1.f);
    m_vertices.push_back(5.f);

    m_vertices.push_back(+m_size);
    m_vertices.push_back(-m_size);
    m_vertices.push_back(+m_size);
    m_vertices.push_back(1.f);
    m_vertices.push_back(1.f);
    m_vertices.push_back(5.f);

    m_vertices.push_back(+m_size);
    m_vertices.push_back(-m_size);
    m_vertices.push_back(-m_size);
    m_vertices.push_back(1.f);
    m_vertices.push_back(0.f);
    m_vertices.push_back(5.f);

    m_vertices.push_back(-m_size);
    m_vertices.push_back(-m_size);
    m_vertices.push_back(-m_size);
    m_vertices.push_back(0.f);
    m_vertices.push_back(0.f);
    m_vertices.push_back(5.f);

    m_indices.push_back(16);
    m_indices.push_back(17);
    m_indices.push_back(18);

    m_indices.push_back(16);
    m_indices.push_back(18);
    m_indices.push_back(19);

    // Top face
    m_vertices.push_back(-m_size);
    m_vertices.push_back(+m_size);
    m_vertices.push_back(-m_size);
    m_vertices.push_back(0.f);
    m_vertices.push_back(1.f);
    m_vertices.push_back(4.f);

    m_vertices.push_back(+m_size);
    m_vertices.push_back(+m_size);
    m_vertices.push_back(-m_size);
    m_vertices.push_back(1.f);
    m_vertices.push_back(1.f);
    m_vertices.push_back(4.f);

    m_vertices.push_back(+m_size);
    m_vertices.push_back(+m_size);
    m_vertices.push_back(+m_size);
    m_vertices.push_back(1.f);
    m_vertices.push_back(0.f);
    m_vertices.push_back(4.f);

    m_vertices.push_back(-m_size);
    m_vertices.push_back(+m_size);
    m_vertices.push_back(+m_size);
    m_vertices.push_back(0.f);
    m_vertices.push_back(0.f);
    m_vertices.push_back(4.f);

    m_indices.push_back(20);
    m_indices.push_back(21);
    m_indices.push_back(22);

    m_indices.push_back(20);
    m_indices.push_back(22);
    m_indices.push_back(23);

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

    glVertexArrayAttribBinding(m_vao, VertexAttrib::TexCoord0, VertexBufferBinding::Slot0);
    glVertexArrayAttribFormat(m_vao, VertexAttrib::TexCoord0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat));
    glEnableVertexArrayAttrib(m_vao, VertexAttrib::TexCoord0);


    GLuint m_texUnit = 15;

    int w, h, comp;
    unsigned char* images[6];

    images[0] = stbi_load("res/space_left.png", &w, &h, &comp, STBI_rgb_alpha);
    images[1] = stbi_load("res/space_front.png", &w, &h, &comp, STBI_rgb_alpha);
    images[2] = stbi_load("res/space_right.png", &w, &h, &comp, STBI_rgb_alpha);
    images[3] = stbi_load("res/space_back.png", &w, &h, &comp, STBI_rgb_alpha);
    images[4] = stbi_load("res/space_top.png", &w, &h, &comp, STBI_rgb_alpha);
    images[5] = stbi_load("res/space_bottom.png", &w, &h, &comp, STBI_rgb_alpha);


    glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &m_texId);
    glTextureStorage3D(m_texId, 1, GL_RGBA8, w, h, 6);
    
    for (int i = 0; i < 6; i++)
        glTextureSubImage3D(m_texId, 0, 0, 0, i, w, h, 1, GL_RGBA, GL_UNSIGNED_BYTE, images[i]);
    glGenerateTextureMipmap(m_texId);

    glCreateSamplers(1, &m_sampler);
    glSamplerParameteri(m_sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(m_sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    m_program->use();
    m_texUnif = glGetUniformLocation(m_program->getProgramId(), "skybox");
    m_program->setUniformSampler(m_texUnif, m_texUnit);

    glBindTextureUnit(m_texUnit, m_texId);
    glBindSampler(m_texUnit, m_sampler);

    for (int i = 0; i < 6; i++)
        stbi_image_free(images[i]);
    

    m_modelMatrix = glm::mat4(1.f);
}

Skybox::~Skybox()
{

}

void Skybox::update(const glm::vec3& cameraPos)
{
    m_modelMatrix = glm::translate(cameraPos);
}

void Skybox::render(const glm::mat4& projection, const glm::mat4& view)
{
    glDisable(GL_DEPTH_TEST);
    m_program->use();
    m_program->setUniformAttribute("MVP", projection * view * m_modelMatrix);
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, nullptr);
    glEnable(GL_DEPTH_TEST);
}

}