#ifndef VERTEX_LOADER_H
#define VERTEX_LOADER_H

#include <string>

class VertexBuffer;

class VertexLoader
{
public:
    VertexLoader();

    void loadFromFile(std::string fileName, VertexBuffer& buffer);

private:
    int loadAttribute(std::string& line, std::string attribute, std::ifstream& stream, VertexBuffer& buffer);
};

#endif // VERTEX_LOADER_H