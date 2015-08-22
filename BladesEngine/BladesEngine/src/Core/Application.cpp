#include <iostream>
#include <fstream>

#include "Core/Application.h"
#include "Core/Timer.h"

#include "Utils/Logger.h"

namespace
{
    int frameLimit = 60;
    __int64 microTimePerFrame = 1000000 / frameLimit;
    float timePerFrame = 1.f / frameLimit;

    const int WindowWidth = 1366;
    const int WindowHeight = 768;
    const int PixelDepth = 32;
}

namespace fsi
{
    Application::Application()
        : m_window(std::make_shared<Window>(VideoMode(WindowWidth, WindowHeight, PixelDepth), "OpenGL Tessellation", Window::Default))
        , m_renderer(std::make_unique<GLRenderer>(m_window))
        //, m_d3dRenderer(std::make_unique<D3DRenderer>(m_window))
    {
    }

    Application::~Application()
    {
    }

    void Application::run()
    {
        Timer frameTimer;
        __int64 timeSinceLastUpdate = 0;

        while (m_window->isOpen())
        {
            __int64 timeDelta = frameTimer.restart();
            timeSinceLastUpdate += timeDelta;

            while (timeSinceLastUpdate > microTimePerFrame)
            {
                processInput();
                m_renderer->update(timePerFrame);
                //m_d3dRenderer->update(timePerFrame);

                timeSinceLastUpdate -= microTimePerFrame;
            }

            auto start = frameTimer.getElapsedTime();
            
            m_renderer->render();
            //m_d3dRenderer->render();
        
            auto timeDiff = frameTimer.getElapsedTime() - start;

            m_renderer->setFrameTime(timeDiff);
        }
    }

    void Application::processInput()
    {
        Event event;
        while (m_window->pollEvent(event))
        {
            m_renderer->handleEvents(event);

            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
                m_window->close();
            else if (event.type == Event::Closed)
                m_window->close();
        }
    }
}
