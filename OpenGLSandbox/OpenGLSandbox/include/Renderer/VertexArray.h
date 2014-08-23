#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include <map>
#include <vector>
#include "OpenGL.h"

// Forward declaration
class VertexBuffer;
class Vertex;

class VertexArray
{
public:
    VertexArray();
    VertexArray(GLenum primitiveType);

    ~VertexArray();

    void attachBuffer(VertexBuffer& buffer);
    void attachBuffers(std::vector<VertexBuffer*> buffers);

    void bind();
    static void release();

    GLuint getId() const;

    void setPrimitiveType(GLenum primitiveType);
    GLenum getPrimitiveType() const;

    void render();

private:
    void create();

    GLuint m_id;
    GLenum m_primitiveType;
    GLsizei m_size;
    std::map<std::string, GLuint> m_attributes;
    std::vector<VertexBuffer*> m_buffers;
};


#endif // VERTEX_ARRAY_H