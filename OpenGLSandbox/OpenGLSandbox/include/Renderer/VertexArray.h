#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include <memory>
#include <map>
#include <vector>
#include "OpenGL.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexAttribute.h"

class VertexArray
{
public:
    VertexArray();
    VertexArray(GLenum primitiveType);

    ~VertexArray();

    void bind();
    static void release();

    void attachBuffer(std::string name, VertexBuffer* buffer);
    void attachIndexBuffer(IndexBuffer* buffer);

    GLuint getId() const;

    void setPrimitiveType(GLenum primitiveType);
    GLenum getPrimitiveType() const;

    void setVertexCount(GLsizei vertexCount);
    GLsizei getVertexCount() const;

    void render();
    void render(GLint first, GLsizei count);
    void renderIndexed();
    void renderIndexed(GLsizei offset);

private:
    void create();

    GLuint m_id;
    GLenum m_primitiveType;
    GLsizei m_vertexCount;

    GLenum m_indexType;
    GLsizei m_indexCount;

    typedef std::shared_ptr<VertexBuffer> VertexBufferPtr;
    typedef std::shared_ptr<VertexAttribute> AttributePtr;
    typedef std::shared_ptr<IndexBuffer> IndexBufferPtr;

    std::map<std::string, VertexBuffer*> m_buffers;
    std::map<std::string, VertexAttribute*> m_attributes;

    IndexBuffer* m_indexBuffer;
};


#endif // VERTEX_ARRAY_H