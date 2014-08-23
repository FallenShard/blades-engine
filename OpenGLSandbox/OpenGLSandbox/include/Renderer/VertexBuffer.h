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

    void create(GLfloat* vertices, int size, int packSize);
    void create(std::vector<GLfloat> vertices, int packSize);
    void loadFromFile(std::string fileName, int packSize);

    void bind() const;
    static void release(GLenum targetType);
 

    GLsizei getSize() const;

    GLuint getId() const;

    GLfloat& operator[](unsigned int index);

private:
    void create();

    GLuint m_id;
    GLenum m_targetType;
    GLenum m_drawType;
    GLint m_packSize;

    std::vector<GLfloat> m_vertices;
};

#endif // VERTEX_BUFFER_H