#include "Core/Application.h"
#include <fstream>
#include "Renderer/Shader.h"

Application::Application()
{
    m_window = new Window(VideoMode(800, 600, 32), "OpenGL Test");
    m_renderer = new GLRenderer();
}

Application::~Application()
{
    delete m_window;
    m_window = nullptr;
}

void Application::run()
{
    while (m_window->isOpen())
    {
        processInput();
        glClearColor(backColor, backColor, backColor, 1.f);
        m_renderer->draw();

        m_window->display();

        Sleep(16);
    }
}

void Application::processInput()
{
    Event event;
    while (m_window->pollEvent(event))
    {
        if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
            m_window->close();
        if (event.type == Event::KeyPressed && event.key.code == Keyboard::A)
            backColor = 1;
        if (event.type == Event::KeyPressed && event.key.code == Keyboard::D)
            backColor = 0;
        if (event.type == Event::Resized)
            backColor = (float) event.size.height / event.size.width;
    }
}

