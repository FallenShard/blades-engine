#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <string>
#include <vector>
#include "OpenGL.h"

template<typename T>
class VertexBuffer
{
public:
    VertexBuffer(GLenum targetType, GLenum drawType);

    ~VertexBuffer();

    void create(T* vertices, int size, int packSize);
    void create(std::vector<T> vertices, int packSize);
    void loadFromFile(std::string fileName, int packSize);

    void bind() const;
    static void release(GLenum targetType);
    static void release(const VertexBuffer<T>& vertexBuffer);
 

    GLsizei getSize() const;

    GLuint getId() const;

    T& operator[](unsigned int index);

private:
    void create();

    GLuint m_id;
    GLenum m_targetType;
    GLenum m_usageType;
    GLint m_packSize;

    unsigned int m_vertexNumber;

    std::vector<T> m_vertexData;
};

typedef VertexBuffer<GLfloat> VertexBufferF;
typedef VertexBuffer<GLshort> VertexBufferS;
typedef VertexBuffer<GLuint>  VertexBufferU;
typedef VertexBuffer<GLint>   VertexBufferI;

#include "VertexBuffer.inl"

#endif // VERTEX_BUFFER_H