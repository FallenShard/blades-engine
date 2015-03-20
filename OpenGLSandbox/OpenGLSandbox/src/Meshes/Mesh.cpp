#include "Meshes/Mesh.h"

namespace fsi
{

Mesh::Mesh()
{

}

Mesh::~Mesh()
{

}

void Mesh::bind()
{
}

const std::vector<VertexAttribute>& Mesh::getVertexAttributes() const
{
    return m_vertexAttributes;
}

}