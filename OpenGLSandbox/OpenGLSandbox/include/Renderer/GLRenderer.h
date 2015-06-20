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
class UIRenderer;

class GLRenderer
{
public:
    GLRenderer(std::shared_ptr<Window>& window);
    ~GLRenderer();

    void handleEvents(const Event& event);
    void update(float timeDelta);
    void draw();

    void resize(int width, int height);

    void setFrameTime(long long frameTime);

    void enableFXAA(bool enabled);

private:
    void init();

    float m_aspectRatio;
    float m_timePassed;

    bool m_FXAAenabled;
    bool m_showGui;

    std::shared_ptr<Window> m_window;
    ShaderManager* m_shaderManager;

    RenderPass* m_aaPass;

    SceneManager* m_sceneManager;
    std::shared_ptr<UIRenderer> m_uiRenderer;
};

}