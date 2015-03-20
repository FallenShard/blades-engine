#pragma once

#include <memory>

#include "Meshes/Mesh.h"
#include "Materials/Material.h"

#include "OglWrapper/VertexArray.h"
#include "OglWrapper/VertexStreams.h"

namespace fsi
{

class RenderComponent
{
public:
    RenderComponent(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material);

    void setPrimitiveType(GLenum primitiveType);

    void render();

    static std::map<VertexAttrib::ID, VertexBufferBinding::ID> bindingTable;

protected:
    void init();

    VertexArray m_vertexArray;

    std::shared_ptr<Mesh> m_mesh;
    std::shared_ptr<Material> m_material;
};


}