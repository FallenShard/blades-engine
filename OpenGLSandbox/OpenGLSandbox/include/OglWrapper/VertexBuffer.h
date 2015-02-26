#pragma once

#include <string>
#include <vector>
#include "OpenGL.h"

namespace fsi
{

class VertexBuffer
{
public:
    // Creates a vertex buffer with GL_ARRAY_BUFFER target and provided usage type
    VertexBuffer(GLenum usageType);

    // Generates a vertex buffer with provided target and usage type
    VertexBuffer(GLenum targetType, GLenum usageType);

    // Deletes a vertex buffer
    ~VertexBuffer();

    // Binds the vertex buffer to its target type
    void bind() const;
    
    // Unbinds any buffer currently bound to GL_ARRAY_BUFFER type
    static void release();

    // Releases any buffer bound to provided target type
    static void release(GLenum targetType); 

    // Releases provided vertex buffer bound to its target type
    static void release(const VertexBuffer& vertexBuffer);

    // Creates a vertex buffer from provided vertex data
    void create(GLfloat* vertexData, unsigned int size);

    // Creates a vertex buffer from provided vector
    void create(std::vector<GLfloat> vertexData);

    // Loads vertex data from specified file
    void loadFromFile(std::string fileName);

    // Pushes a single float value into the vertex buffer
    void push(GLfloat value);

    // Pushes two values into the buffer
    void push(GLfloat x, GLfloat y);

    // Pushes three values into the buffer
    void push(GLfloat x, GLfloat y, GLfloat z);

    // Pushes four values into the buffer
    void push(GLfloat x, GLfloat y, GLfloat z, GLfloat w);

    // Uploads buffer data to the GPU
    void uploadData();

    // Clears the buffer
    void clear();

    // Sets the number in bytes that each vertex uses
    void setDataCountPerVertex(GLsizei dataCountPerVertex);

    // Returns vertex count
    GLsizei getVertexCount() const;

    // Returns OpenGL ID of the vertex buffer
    GLuint getBufferId() const;

    // Returns an element in vertex buffer with specified index
    GLfloat& operator[](unsigned int index);

private:
    GLuint m_id;

    GLenum m_targetType;
    GLenum m_usageType;

    GLsizei m_dataCountPerVertex;
    std::vector<GLfloat> m_vertexData;
};

}