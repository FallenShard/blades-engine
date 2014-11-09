#pragma once

#include "OglWrapper/VertexArray.h"
#include "OglWrapper/VertexBuffer.h"
#include "OglWrapper/ShaderProgram.h"

#include "Models/SceneNode.h"

class PlaneGrid : public SceneNode
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

    virtual void render(const glm::mat4& projection, const glm::mat4& view);

private:
    void buildVertices();

    float        m_size;
    float        m_lineStep;
    int          m_visiblePlanes;

    VertexBuffer m_vertexBuffer;
};