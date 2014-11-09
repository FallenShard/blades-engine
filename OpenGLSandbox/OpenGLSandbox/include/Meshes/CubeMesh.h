#pragma once

#include "OglWrapper/VertexArray.h"
#include "OglWrapper/VertexBuffer.h"
#include "OglWrapper/IndexBuffer.h"

class PrismMesh
{
public:
    PrismMesh();

    void bindToArray(VertexArray* vArray);
    VertexArray* getVertexArray();

    ~PrismMesh();

private:
    void init();

    VertexArray m_vertexArray;
    VertexBuffer m_vertexBuffer;
    IndexBuffer m_indexBuffer;
};