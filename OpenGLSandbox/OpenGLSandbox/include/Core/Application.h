#pragma once

#include "Window/Window.h"
#include "Renderer/GLRenderer.h"

class Application
{
public:
    Application();
    ~Application();

    void run();

private:
    void processInput();
    void render();
    void renderInit();

    Window* m_window;
    GLRenderer* m_renderer;

    float backColor;
};