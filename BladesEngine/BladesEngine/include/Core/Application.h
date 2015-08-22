#pragma once

#include "Window/Window.h"
#include "Renderer/GLRenderer.h"
#include "Renderer/D3DRenderer.h"

namespace fsi
{
    class Application
    {
    public:
        Application();
        ~Application();

        void run();

    private:
        void processInput();

        std::shared_ptr<Window> m_window;
        std::unique_ptr<GLRenderer> m_renderer;
        std::unique_ptr<D3DRenderer> m_d3dRenderer;
    };
}