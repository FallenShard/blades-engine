#pragma once

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
#include "Window/Window.h"

struct Event;

class GLRenderer
{
public:
    GLRenderer(Window* window);
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

    Window* m_window;

    std::vector<Scene*> m_scenes;
};