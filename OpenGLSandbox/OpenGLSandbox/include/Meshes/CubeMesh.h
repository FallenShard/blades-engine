#pragma once

#include "OglWrapper/VertexArray.h"
#include "OglWrapper/VertexBuffer.h"
#include "OglWrapper/IndexBuffer.h"

namespace fsi
{

class CubeMesh
{
public:
    CubeMesh();

    void bindToArray(VertexArray* vArray);
    VertexArray* getVertexArray();

    ~CubeMesh();

private:
    void init();

    VertexArray m_vertexArray;
    VertexBuffer m_vertexBuffer;
    IndexBuffer m_indexBuffer;
};

}