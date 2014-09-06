#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <memory>
#include <string>
#include <vector>
#include <map>
#include "OpenGL.h"
#include "VertexAttribute.h"

class VertexBuffer
{
public:
    typedef std::shared_ptr<VertexAttribute> AttributePtr;
    typedef std::map<std::string, AttributePtr> AttributeMap;

    VertexBuffer(GLenum targetType, GLenum drawType);

    ~VertexBuffer();

    void bind() const;
    static void release(GLenum targetType);
    static void release(const VertexBuffer& vertexBuffer);

    void create(GLfloat* vertices, unsigned int size);
    void create(std::vector<GLfloat> vertices);
    void loadFromFile(std::string fileName);

    GLsizei getSize() const;
    GLsizei getVertexCount() const;

    GLuint getId() const;

    GLfloat& operator[](unsigned int index);

    AttributeMap& getAttributeMap();

private:
    int loadAttributeFromFile(std::string& line, std::string attribute, std::ifstream& stream);
    void registerAttribute(std::string name, GLint dataPerAttribute, GLint offset);

    GLuint m_id;
    GLenum m_targetType;
    GLenum m_usageType;
    GLsizei m_vertexCount;

    std::vector<GLfloat> m_vertexData;

    AttributeMap m_vertexAttributes;
};

#endif // VERTEX_BUFFER_H