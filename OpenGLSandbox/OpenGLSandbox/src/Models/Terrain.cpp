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

}

namespace fsi
{

Terrain::Terrain()
{
}

Terrain::Terrain(PlaneMesh* mesh, HeightMapMaterial* material)
    //: m_renderComp(std::make_unique<RenderComponent>(mesh, material))
    : m_mesh(mesh)
    , m_material(material)
    , m_program(material->getShaderProgram())
    , m_vertexArray(GL_PATCHES)
{
    //m_renderComp->setPrimitiveType(GL_PATCHES);
}

Terrain::~Terrain()
{
}


void Terrain::init()
{
    m_vertexArray.bind();

    m_mesh->bind();

    const std::vector<VertexAttribute>& attribs = m_mesh->getVertexAttributes();

    for (auto attribute : attribs)
    {
        m_vertexArray.enableVertexAttrib(attribute.attribLoc);
        m_vertexArray.attributeFormat(attribute);
        m_vertexArray.attributeBinding(attribute.attribLoc, RenderComponent::bindingTable[attribute.attribLoc]);
    }

    m_vertexArray.setVertexCount(m_mesh->getVertexCount());
    m_vertexArray.setIndexCount(m_mesh->getIndexCount());

    m_material->init();

    int w;
    int h;
    int comp;

    m_program->use();
    //m_program->setUniformSampler(hMap)
    //glUniform1i(hMap, 1);

    hMap = glGetUniformLocation(m_program->getProgramId(), "hMap");
    

    unsigned char* img = stbi_load("res/heightmap2.png", &w, &h, &comp, STBI_rgb_alpha);

    glGenTextures(1, &texId);

    glBindTexture(GL_TEXTURE_2D, texId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    stbi_image_free(img);

    int maxVert;
    glGetIntegerv(GL_MAX_TESS_GEN_LEVEL, &maxVert);
    std::cout << maxVert << '\n';
}

void Terrain::update(const float deltaTime)
{
    timeVal += deltaTime;
}

void Terrain::render(const glm::mat4& projection, const glm::mat4& view)
{
    glm::mat4 model = glm::translate(glm::vec3(0.f, -50.f, 0.f));
    glm::mat4 mat = projection * view * model;
    glm::mat3& normalMatrix = glm::mat3(glm::inverseTranspose(view * model));

    glm::vec4 cameraPos = glm::inverse(view)[3];

    //std::cout << cameraPos.x << " " << cameraPos.y << " " << cameraPos.z << '\n';

    m_program->use();
    m_program->setUniformAttribute("normalMatrix", normalMatrix);
    m_program->setUniformAttribute("MVP", mat);
    m_program->setUniformAttribute("cameraPos", cameraPos);
    m_program->setUniformAttribute("time", timeVal);
    m_program->setUniformAttribute("mvLightDir", view * glm::vec4(0.f, -1.f, 0.f, 0.f));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texId);
    m_program->setUniformSampler(hMap, 0);

    glLineWidth(3.f);

    m_program->setUniformAttribute("wireframe", false);

    m_vertexArray.bind();
    m_vertexArray.renderIndexed();

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    m_program->setUniformAttribute("wireframe", true);
    //m_program->setUniformAttribute("MVP", projection * view * glm::translate(glm::vec3(0.f, -49.5f, 0.f)));

    m_vertexArray.bind();
    m_vertexArray.renderIndexed();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

}