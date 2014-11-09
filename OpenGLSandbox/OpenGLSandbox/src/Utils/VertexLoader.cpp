#include <fstream>
#include <sstream>
#include <algorithm>
#include <numeric>

#include "Utils/VertexLoader.h"
#include "OglWrapper/VertexBuffer.h"
#include "OglWrapper/VertexArray.h"

VertexLoader::VertexLoader()
{
}

void VertexLoader::loadFromFile(std::string fileName, VertexBuffer& buffer)
{
    std::ifstream inputFile(fileName, std::ios::in);
    std::string line;

    // Load the vertex data
    int dataPerVertexCount = 0;
    int dataPerAttribute = 0;
    int lineDataCounter = 0;
    int vertexCount = 0;

    GLfloat vertexData;

    // Read the file lines one by one and register the attributes
    while (std::getline(inputFile, line))
    {
        std::stringstream lineStream(line);

        lineDataCounter = 0;
        // Check if a float value could be read
        while (lineStream >> vertexData)
        {
            m_vertexData.push_back(vertexData);
            lineDataCounter++;
        }

        // If there was no data, check if the string wasn't empty
        if (lineDataCounter == 0)
        {
            // If it wasn't empty, it marked a new attribute
            if (line != "" && dataPerAttribute != 0)
            {
                m_attributeSizes.push_back(dataPerAttribute);
                dataPerAttribute = 0;
            }
        }
        else
        {
            // If there was data, compute current data per attribute
            dataPerAttribute = std::max(dataPerAttribute, lineDataCounter);
        }
    }
    inputFile.close();
    m_attributeSizes.push_back(dataPerAttribute);

    dataPerVertexCount = std::accumulate(m_attributeSizes.begin(), m_attributeSizes.end(), 0);
    vertexCount = m_vertexData.size() / dataPerVertexCount;

    m_attributeOffsets.push_back(0);
    int i = 0;
    while (m_attributeOffsets.size() < m_attributeSizes.size())
    {
        m_attributeOffsets.push_back(m_attributeOffsets[i] + m_attributeSizes[i] * sizeof(GLfloat) * vertexCount);
        i++;
    }

    buffer.create(m_vertexData);
    buffer.setDataCountPerVertex(dataPerVertexCount);
}

std::vector<int> VertexLoader::getAttributeSizes()
{
    return m_attributeSizes;
}

std::vector<int> VertexLoader::getAttributeOffsets()
{
    return m_attributeOffsets;
}