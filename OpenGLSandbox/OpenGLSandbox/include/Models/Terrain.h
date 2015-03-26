#pragma once

#include <memory>

#include "OpenGL.h"

#include "OglWrapper/VertexArray.h"

#include "Meshes/PlaneMesh.h"
#include "Materials/HeightMapMaterial.h"

namespace fsi
{

class RenderComponent;
class TransformComponent;

class Terrain // : public Entity
{
public:
    Terrain();
    Terrain(PlaneMesh* mesh, HeightMapMaterial* material, ShaderProgram* prog);

    ~Terrain();


    void init();

    void update(const float deltaTime);


    void render(const glm::mat4& projection, const glm::mat4& view);

private:
    //VertexArray m_vertexArray;
    //std::shared_ptr<PlaneMesh> m_mesh;
    std::shared_ptr<HeightMapMaterial> m_material;
    //std::unique_ptr<RenderComponent> m_renderComp;
    ShaderProgram* m_program;

    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ibo;
    GLuint m_tex;
    GLuint m_sampler;

    glm::mat4 m_modelMatrix;
};

}