#pragma once

#include <memory>

#include "OpenGL.h"

namespace fsi
{
    class Window;

    class GLContext
    {
    public:
        GLContext(std::shared_ptr<Window>& window);
        ~GLContext();

        void attachWindow(std::shared_ptr<Window>& window);

    private:
        std::shared_ptr<Window> m_window;
        HGLRC m_resourceContext;
    };
}