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

    std::shared_ptr<Window> m_window;
    std::unique_ptr<GLRenderer> m_renderer;
};