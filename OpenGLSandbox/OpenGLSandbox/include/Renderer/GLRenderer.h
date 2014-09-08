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
    float m_timePassed;

    std::vector<Scene*> m_scenes;
};

#endif // GL_RENDERER_H