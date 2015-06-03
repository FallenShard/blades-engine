#pragma once

#include <vector>
#include <map>

#include "OpenGL.h"
#include "OglWrapper/Shader.h"
#include "OglWrapper/ShaderProgram.h"
#include "Renderer/ShaderManager.h"
#include "PostProcessing/RenderPass.h"
#include "Renderer/SceneManager.h"
#include "Window/Window.h"

namespace fsi
{

struct Event;
class TextRenderer;

class GLRenderer
{
public:
    GLRenderer(Window* window);
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

    SceneManager* m_sceneManager;
    TextRenderer* m_textRenderer;
};

}