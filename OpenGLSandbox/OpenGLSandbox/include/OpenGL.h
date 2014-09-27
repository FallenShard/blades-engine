#pragma once

#include "GL/gl3w.h"
#include "GL/glcorearb.h"
#include "GL/wglext.h"

#include "glm.hpp"
#include "ext.hpp"

/*
        Vertex Array functions
    
    glGenVertexArrays(GLsizei n, GLuint* arrays); Vertex array hold various data related to vertices and they have multiple VBOs (Vertex Buffer Objects)
    glBindVertexArray(GLuint array); -- 0 unbinds any currently bound array, nonzero binds a new array which was generated at first
    glDeleteVertexArrays(GLsizei n, GLuint* arrays);
    glIsVertexArray(GLuint array); GL_TRUE/GL_FALSE whether array the integer is a vertex array identifier
    
    
        Vertex Buffer functions
    
    glGenBuffers(GLsizei n, GLuint* buffers); Generates n new buffers for bound vertex array
    glBindBuffer(GLenum target, GLuint buffer); Binds a buffer to use, target is the type, buffer is generated identifier
        target can be:
            GL_ARRAY_BUFFER, usually for vertex positions/attributes data
            GL_ELEMENT_ARRAY_BUFFER, for index data
            GL_PIXEL_PACK_BUFFER,
            GL_PIXEL_UNPACK_BUFFER,
            GL_COPY_READ_BUFFER,
            GL_COPY_WRITE_BUFFER,
            GL_TRANSFORM_FEEDBACK_BUFFER,
            GL_UNIFORM_BUFFER
    glDeleteBuffers(GLsizei n, const GLuint* buffers);
    glIsBuffer(GLuint buffer);


        Loading data into buffer object

    glBufferData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage);
        target can be:
            GL_ARRAY_BUFFER, usually for vertex positions/attributes data
            GL_ELEMENT_ARRAY_BUFFER, for index data
            GL_PIXEL_PACK_BUFFER, for pixel data passed to OpenGL
            GL_PIXEL_UNPACK_BUFFER, for pixel data retrieved from OpenGL
            GL_COPY_READ_BUFFER, data copy between buffers
            GL_COPY_WRITE_BUFFER, data copy between buffers
            GL_TEXTURE_BUFFER, for texture data
            GL_TRANSFORM_FEEDBACK_BUFFER, for results from transform feedback shader
            GL_UNIFORM_BUFFER, for uniform variable values
        size = sizeof(data)
        data can be:
            NULL, space is left uninitialized, but reserved with size
            Actual data in client memory
        usage can be:
            GL_STREAM_DRAW,
            GL_STREAM_READ,
            GL_STREAM_COPY,
            GL_STATIC_DRAW,
            GL_STATIC_READ,
            GL_STATIC_COPY,
            GL_DYNAMIC_DRAW,
            GL_DYNAMIC_READ,
            GL_DYNAMIC_COPY, 
    
        
        Vertex Attributes

    glGetAttribLocation(GLuint program, const GLchar* var); returns attribute index of a variable name
    glEnableVertexAttribArray(GLuint array); enables attributes for array
    glDisableVertexAttribArray(GLuint array); disables attributes for array
    glVertexAttribPointer(GLuint index, GLint size, GLenum type, 
                          GLboolean normalized, GLsizei stride, const GLvoid* pointer);
        index is vertex attribute id
        size is number of components per vertex (1, 2, 3, 4 or BGRA)
        type is actual type of passed data (usually GL_FLOAT, can be any required type)
        normalized - whether data should be normalized before being stored
        stride is byte offset between elements of passed data
        pointer is offset in bytes before the start of bytes to use
        



        OpenGL State Calls

    glClear(GLbitfield mask); called before rendering a new frame
        mask can be GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT or GL_STENCIL_BUFFER_BIT
    glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
    


        OpenGL Draw Calls

    glDrawArrays(GLenum mode, GLint first, GLsizei count); call on currently bound vertex array
        mode can be:
            GL_POINTS, for points
            GL_LINES, for lines
            GL_LINE_STRIP, for segmented lines
            GL_LINE_LOOP, for hollow figures
            GL_TRIANGLES, for triangles
            GL_TRIANGLE_STRIP, for triangle strips
            GL_TRIANGLE_FAN, for pie-like triangle shapes
            GL_PATCHES, for tessellation shading only



        Viewport Transforms

    glViewport(GLint x, GLint y, GLsizei width, GLsizei height); usually called with 0, 0, newWindowWidth, newWindowHeight

*/