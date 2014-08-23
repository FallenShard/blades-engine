#ifndef GL_RENDERER_H
#define GL_RENDERER_H

#include <vector>
#include <unordered_map>

#include "OpenGL.h"
#include "Renderer/ShaderProgram.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/VertexAttribute.h"

class GLRenderer
{
public:
    GLRenderer();
    GLRenderer(int width, int height);
    ~GLRenderer();

    

    void update(float timeDelta);
    void draw();

    void resize(int width, int height);

private:
    void init();
    void shaderSetup();

    float m_aspectRatio;
    ShaderProgram m_shaderProgram;
    ShaderProgram m_defaultProgram;

    std::vector<VertexArray*> m_vertexArrays;
    std::vector<VertexBuffer*> m_vertexBuffers;
    std::vector<VertexAttribute*> m_vertexAttributes;
    
    float m_timePassed;
};


#endif // GL_RENDERER_H