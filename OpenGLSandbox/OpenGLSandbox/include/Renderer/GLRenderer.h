#ifndef GL_RENDERER_H
#define GL_RENDERER_H

#include "OpenGL.h"
#include "Renderer/ShaderProgram.h"
#include "Renderer/Shader.h"

class GLRenderer
{
public:
    GLRenderer();

    void init();

    void draw();

private:
    ShaderProgram m_shaderProgram;

};


#endif // GL_RENDERER_H