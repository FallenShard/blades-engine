#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <string>
#include <vector>
#include "OpenGL.h"

class VertexBuffer
{
public:
    VertexBuffer(GLenum targetType, GLenum drawType);

    ~VertexBuffer();

    void bind() const;
    static void release(GLenum targetType);
    static void release(const VertexBuffer& vertexBuffer);

    void create(GLfloat* vertices, int size);
    void create(std::vector<GLfloat> vertices);
    void loadFromFile(std::string fileName);

    void registerAttributeSize(int dataPerVertexAttribute);

    GLsizei getSize() const;
    GLsizei getVertexAmount() const;

    GLuint getId() const;

    GLfloat& operator[](unsigned int index);

private:
    GLuint m_id;
    GLenum m_targetType;
    GLenum m_usageType;
    GLint m_dataPerVertex;

    std::vector<GLfloat> m_vertexData;
};

#endif // VERTEX_BUFFER_H