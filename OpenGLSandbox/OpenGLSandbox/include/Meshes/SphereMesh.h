#pragma once

#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"

class SphereMesh
{
public:
    SphereMesh();
    SphereMesh(float radius, short int rings = 20, short int slices = 20);

    void bindToArray(VertexArray* vArray);
    VertexArray* getVertexArray();

    ~SphereMesh();

private:
    void computeVertices();

    float m_radius;
    short int m_rings;
    short int m_slices;

    VertexArray m_vertexArray;
    VertexBuffer m_vertexBuffer;
    IndexBuffer m_indexBuffer;
};