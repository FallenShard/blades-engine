#pragma once

#include <set>

#include "OglWrapper/VertexFormat.h"
#include "OglWrapper/VertexArray.h"
#include "Meshes/Mesh.h"

#include "RenderPackage.h"

namespace fsi
{

class BufferManager
{
public:
    BufferManager();
    ~BufferManager();

    RenderPackage submitMesh(Mesh* mesh);


private:
    static std::map<VertexAttrib::ID, VertexBufferBinding::ID> bindingTable;

    std::set<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
    std::set<std::shared_ptr<IndexBuffer>> m_indexBuffers;
    //std::map<GLuint, std::shared_ptr<VertexBuffer>> m_vertexBuffers;
    std::map<GLuint, std::shared_ptr<VertexFormat>> m_vertexFormats;
};

}
