#include <fstream>

#include "Core/Application.h"
#include "Core/Timer.h"

#include "Utils/Logger.h"

namespace
{
    int frameLimit = 60;
    __int64 microTimePerFrame = 1000000 / frameLimit;
    float timePerFrame = 1.f / frameLimit;

    const int windowWidth = 1366;
    const int windowHeight = 768;
}

namespace fsi
{

Application::Application()
    : m_window(std::make_shared<Window>(VideoMode(windowWidth, windowHeight, 32), "OpenGL Tessellation"))
    , m_renderer(std::make_unique<GLRenderer>(m_window.get()))
{
}

Application::~Application()
{
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
        m_renderer->handleEvents(event);

        if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
            m_window->close();

        if (event.type == Event::Closed)
            m_window->close();
    }
}

}
