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
#include "Scenes/Scene.h"

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

    typedef std::unique_ptr<ShaderProgram> ProgramPtr;
    typedef std::unique_ptr<VertexArray> VertexArrayPtr;
    typedef std::unique_ptr<VertexBuffer> VertexBufferPtr;
    typedef std::unique_ptr<VertexBuffer> IndexBufferPtr;
    typedef std::unique_ptr<VertexAttribute> AttributePtr;

    std::map<std::string, ProgramPtr> m_shaderPrograms;
    std::map<std::string, VertexArrayPtr> m_vertexArrays;
    std::map<std::string, VertexBufferPtr> m_vertexBuffers;
    std::map<std::string, IndexBufferPtr> m_indexBuffers;
    std::map<std::string, AttributePtr> m_vertexAttributes;

    std::vector<Scene*> m_scenes;
    
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