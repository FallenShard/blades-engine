#ifndef GL_RENDERER_H
#define GL_RENDERER_H

#include <vector>
#include <map>

#include "OpenGL.h"
#include "Renderer/ShaderProgram.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/VertexAttribute.h"

struct Event;

class GLRenderer
{
public:
    GLRenderer();
    GLRenderer(int width, int height);
    ~GLRenderer();

    void handleEvents(const Event& event);
    void update(float timeDelta);
    void draw();

    void resize(int width, int height);

private:
    void init();

    float m_aspectRatio;

    std::map<std::string, ShaderProgram*> m_shaderPrograms;
    std::map<std::string, VertexArray*> m_vertexArrays;
    std::map<std::string, VertexBuffer*> m_vertexBuffers;
    std::map<std::string, IndexBuffer*> m_indexBuffers;
    std::vector<VertexAttribute*> m_vertexAttributes;
    
    float m_timePassed;

    void prepareTriangleScene();
    void drawTriangleScene();
    
    void preparePrismScene();
    void drawPrismScene();

    void prepareOverlapScene();
    void drawOverlapScene();

    void prepareMotionScene();
    void drawMotionScene();
};


#endif // GL_RENDERER_H