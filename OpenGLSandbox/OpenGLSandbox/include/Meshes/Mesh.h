#pragma once

#include <memory>

#include "OglWrapper/VertexBuffer.h"
#include "OglWrapper/IndexBuffer.h"
#include "OglWrapper/VertexAttribute.h"

namespace fsi
{

class Mesh
{
public:
    Mesh();

    virtual void bind() = 0;
    virtual unsigned int getVertexCount() = 0;
    virtual unsigned int getIndexCount() = 0;

    const std::vector<VertexAttribute>& getVertexAttributes() const;

    virtual ~Mesh();

protected:
    std::unique_ptr<VertexBuffer> m_vertexBuffer;
    std::unique_ptr<IndexBuffer> m_indexBuffer;

    std::vector<VertexAttribute> m_vertexAttributes;

};

}