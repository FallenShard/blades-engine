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

    void setData(GLfloat* vertices, int size, int packSize);
    void setData(std::vector<GLfloat> vertices, int packSize);
    void loadFromFile(std::string fileName);

    void bind() const;

    GLsizei getSize() const;

    GLuint getId() const;

    GLfloat& operator[](int index);

private:
    void create();

    GLuint m_id;
    GLenum m_targetType;
    GLenum m_drawType;
    GLint m_packSize;

    std::vector<GLfloat> m_vertices;
};

#endif // VERTEX_BUFFER_H