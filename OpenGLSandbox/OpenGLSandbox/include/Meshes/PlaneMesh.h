#pragma once

#include "Meshes/Mesh.h"

namespace fsi
{

class PlaneMesh : public Mesh
{
public:
    PlaneMesh(unsigned int xSquares, unsigned int zSquares, float squareSize);

    ~PlaneMesh();

    virtual void bind();
    virtual unsigned int getVertexCount();
    virtual unsigned int getIndexCount();

private:
    void initVertices();

    unsigned int m_xSquares;
    unsigned int m_zSquares;
    float m_squareSize;

    float m_xTotalSize;
    float m_zTotalSize;
    
};

}