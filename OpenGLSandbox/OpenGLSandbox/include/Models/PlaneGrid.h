#pragma once

#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/ShaderProgram.h"


class PlaneGrid
{
public:
    enum Planes
    {
        XZ = 1 << 0,
        XY = 1 << 1,
        YZ = 1 << 2
    };

    PlaneGrid(float size, float lineStep, int planes = XZ);
    ~PlaneGrid();

    void setProgram(ShaderProgram* program);
    void displayPlanes(int planes);

    void render();

private:
    void buildVertices();

    float        m_size;
    float        m_lineStep;
    int          m_visiblePlanes;

    VertexBuffer m_vertexBuffer;
    VertexArray  m_vertexArray;
    ShaderProgram* m_program;
};