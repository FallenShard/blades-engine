#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include <string>
#include <vector>
#include "OpenGL.h"

class IndexBuffer
{
public:
    IndexBuffer(GLenum targetType, GLenum drawType);

    ~IndexBuffer();
    
    void bind() const;
    static void release(GLenum targetType);
    static void release(const IndexBuffer& indexBuffer);

    void create(GLshort* indices, int size);
    void create(std::vector<GLshort> indices);
    void loadFromFile(std::string fileName);

    GLsizei getSize() const;
    GLuint getId() const;

    GLshort& operator[](unsigned int index);

private:
    GLuint m_id;
    GLenum m_targetType;
    GLenum m_usageType;

    std::vector<GLshort> m_indices;
};

#endif // INDEX_BUFFER_H