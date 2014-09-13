#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include <map>
#include "OpenGL.h"
#include "VertexAttribute.h"
#include "IndexBuffer.h"

class VertexBuffer;

class VertexArray
{
public:
    // Generates a vertex array with GL_TRIANGLES primitive
    VertexArray();

    // Generates a vertex array with provided primitive type
    VertexArray(GLenum primitiveType);

    // Deletes a vertex array
    ~VertexArray();

    // Binds the vertex array to current OpenGL context
    void bind();

    // Unbinds any currently bound vertex array
    static void release();

    // Returns OpenGL ID of the vertex array
    GLuint getVertexArrayId() const;

    // Sets the vertex count for the vertex array, usually received from vertex buffer
    void setVertexCount(GLsizei vertexCount);

    // Returns vertex count of the vertex array
    GLsizei getVertexCount() const;

    // Calculates per vertex data count
    GLsizei getPerVertexDataCount() const;

    // Attaches a vertex buffer to the vertex array
    void attachBuffer(std::string name, VertexBuffer* buffer);

    // Attaches an index buffer to provide index list for vertices
    void attachIndexBuffer(IndexBuffer* buffer);

    // Attaches an attribute associated with this vertex array to describe format of its buffers
    void attachAttribute(VertexAttribute attribute);

    // Enables all the attached attributes on a given shader program ID
    void enableAttributes(GLuint programId);

    // Renders the array with set vertex count and no offset
    void render();

    // Renders the array with given index for first vertex and a count
    void render(GLint first, GLsizei count);

    // Renders using index data
    void renderIndexed();

    // Renders using index data with an offset on indices
    void renderIndexed(GLsizei offset);

private:
    GLuint m_id;

    GLenum m_primitiveType;
    GLsizei m_vertexCount;

    GLenum m_indexType;
    GLsizei m_indexCount;

    std::map<std::string, VertexBuffer*> m_buffers;
    std::map<std::string, VertexAttribute> m_attributes;
    IndexBuffer* m_indexBuffer;
};

#endif // VERTEX_ARRAY_H