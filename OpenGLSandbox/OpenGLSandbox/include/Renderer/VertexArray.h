#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include <map>
#include <vector>
#include "OpenGL.h"
#include "VertexBuffer.h"

// Forward declaration
class Vertex;

class VertexArray
{
public:
    VertexArray();
    VertexArray(GLenum primitiveType);

    ~VertexArray();

    void attachBuffer(VertexBufferF& buffer);
    void attachBuffers(std::vector<VertexBufferF*> buffers);

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
    std::vector<VertexBufferF*> m_buffers;
};


#endif // VERTEX_ARRAY_H