#pragma once

#include <vector>
#include <map>

#include "OpenGL.h"
#include "OglWrapper/Shader.h"
#include "OglWrapper/ShaderProgram.h"
#include "Renderer/ShaderManager.h"
#include "OglWrapper/VertexArray.h"
#include "OglWrapper/VertexBuffer.h"
#include "OglWrapper/IndexBuffer.h"
#include "OglWrapper/VertexAttribute.h"
#include "Renderer/RenderPass.h"
#include "Renderer/Scene.h"
#include "Window/Window.h"

namespace fsi
{

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
    ShaderManager* m_shaderManager;

    RenderPass* m_aaPass;

    Scene* m_scene;
};

}