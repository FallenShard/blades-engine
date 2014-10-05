#pragma once

#include <string>
#include <vector>
#include "OpenGL.h"

class VertexBuffer;

class VertexLoader
{
public:
    VertexLoader();

    void loadFromFile(std::string fileName, VertexBuffer& buffer);
    std::vector<int> getAttributeSizes();
    std::vector<int> getAttributeOffsets();

private:
    std::vector<int> m_attributeSizes;
    std::vector<int> m_attributeOffsets;

    std::vector<GLfloat> m_vertexData;
};