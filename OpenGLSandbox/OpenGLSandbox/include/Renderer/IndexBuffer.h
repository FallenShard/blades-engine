#pragma once

#include <string>
#include <vector>
#include "OpenGL.h"

class IndexBuffer
{
public:
    // Creates an index buffer with GL_ELEMENT_ARRAY_BUFFER target and GL_STATIC_DRAW usage
    IndexBuffer();

    // Creates an index buffer with GL_ELEMENT_ARRAY_BUFFER target and specified usage
    IndexBuffer(GLenum usageType);

    // Creates an index buffer with provided target and usage types
    IndexBuffer(GLenum targetType, GLenum usageType);

    // Deletes the index buffer
    ~IndexBuffer();
    
    // Binds the index buffer to its target type
    void bind() const;
    
    // Unbinds any buffer bound to GL_ELEMENT_ARRAY_BUFFER target
    static void release();
    // Unbinds any buffer bound to provided target type
    static void release(GLenum targetType);

    // Unbinds any index buffer bound to its target type
    static void release(const IndexBuffer& indexBuffer);

    // Fills an index buffer with provided indices
    void create(GLshort* indices, unsigned int size);

    // Fills an index buffer with provided indices
    void create(std::vector<GLshort> indices);

    // Loads index buffer data from specified file
    void loadFromFile(std::string fileName);

    // Returns number of total indices from index buffer
    GLsizei getSize() const;

    // Returns OpenGL ID of index buffer
    GLuint getIndexBufferId() const;

    // Returns an element in index buffer specified by index
    GLshort& operator[](unsigned int index);

private:
    GLuint m_id;

    GLenum m_targetType;
    GLenum m_usageType;

    std::vector<GLshort> m_indices;
};