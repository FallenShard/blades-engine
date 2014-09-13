#include <fstream>
#include <sstream>

#include "Utils/VertexLoader.h"
#include "Renderer/VertexBuffer.h"

VertexLoader::VertexLoader()
{

}
/*
void VertexLoader::loadFromFile(std::string fileName, VertexBuffer& buffer)
{
    std::ifstream inputFile(fileName, std::ios::in);
    std::string line;

    // Load the vertex data
    int dataPerVertexCount = 0;

    int positionDataCount = 0;
    int colorDataCount = 0;

    // Read the file lines one by one and register the attributes
    while (std::getline(inputFile, line))
    {
        positionDataCount = loadAttribute(line, "Vertices", inputFile, buffer);
        dataPerVertexCount += positionDataCount;

        colorDataCount = loadAttribute(line, "Colors", inputFile, buffer);
        dataPerVertexCount += colorDataCount;
    }

    // Calculate vertex count
    //m_vertexCount = m_vertexData.size() / dataPerVertexCount;

    //registerAttribute("vPosition", positionDataCount, 0);

    //GLint colorOffset = m_vertexCount * positionDataCount * sizeof GLfloat;
    //registerAttribute("vColor", colorDataCount, colorOffset);
}

int VertexLoader::loadAttribute(std::string& line, std::string attribute, std::ifstream& stream, VertexBuffer& buffer)
{
    GLfloat vertexData;
    int attributeDataCount = 0;
    int dataCounter = 0;

    // Try to find attribute header
    if (line.find(attribute) != std::string::npos)
    {
        // If so, continue reading file line by line
        while (std::getline(stream, line))
        {
            std::stringstream lineStream(line);

            // Read all coordinates one by one per line
            dataCounter = 0;
            while (lineStream >> vertexData)
            {
                m_vertexData.push_back(vertexData);
                dataCounter++;
            }

            // Remember the highest seen per vertex attribute data
            attributeDataCount = std::max(attributeDataCount, dataCounter);

            // End reading session if there's no data anymore for this attribute
            if (dataCounter == 0 && line != "")
                break;
        }
    }

    return attributeDataCount;
}*/