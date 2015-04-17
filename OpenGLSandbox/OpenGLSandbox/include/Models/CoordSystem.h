#pragma once

#include "Models/SceneNode.h"

#include "OglWrapper/VertexBuffer.h"
#include "OglWrapper/IndexBuffer.h"

#include "Materials/Material.h"

#include "Meshes/SphereMesh.h"

namespace fsi
{

    class CoordSystem : public SceneNode
    {
    public:
        CoordSystem();
        CoordSystem(SphereMesh* mesh, ShaderProgram* program);
        CoordSystem(SphereMesh* mesh, ShaderProgram* program, Material* material);

        ~Sphere();

        virtual void render(const glm::mat4& projection, const glm::mat4& view);
        void setPrimitiveType(GLenum type);

    private:
        void init();

        SphereMesh* m_mesh;
        Material* m_material;
    };

}