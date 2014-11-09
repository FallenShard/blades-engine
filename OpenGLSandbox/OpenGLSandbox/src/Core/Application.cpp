#include "Core/Application.h"
#include "Core/Timer.h"
#include <fstream>

#include "Utils/Logger.h"

namespace
{
    int frameLimit = 60;
    __int64 microTimePerFrame = 1000000 / frameLimit;
    float timePerFrame = 1.f / frameLimit;

    const int windowWidth = 1366;
    const int windowHeight = 768;
}

Application::Application()
{
    m_window = new Window(VideoMode(windowWidth, windowHeight, 32), "OpenGL Test");
    m_renderer = new GLRenderer(m_window);
    backColor = 0.f;
}

Application::~Application()
{
    delete m_window;
    m_window = nullptr;

    delete m_renderer;
    m_renderer = nullptr;
}

void Application::run()
{
    Timer frameTimer;
    __int64 timeSinceLastUpdate = 0;

    m_renderer->resize(windowWidth, windowHeight);

    while (m_window->isOpen())
    {
        __int64 timeDelta = frameTimer.restart();
        timeSinceLastUpdate += timeDelta;

        while (timeSinceLastUpdate > microTimePerFrame)
        {
            processInput();
            m_renderer->update(timePerFrame);

            timeSinceLastUpdate -= microTimePerFrame;    
        }

        m_renderer->draw();
        m_window->display();
    }
}

void Application::processInput()
{
    Event event;
    while (m_window->pollEvent(event))
    {
        if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
            m_window->close();

        m_renderer->handleEvents(event);
    }
}

