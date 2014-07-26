#ifndef APP_H
#define APP_H

#include "Window/Window.h"

class Application
{
public:
    Application();
    ~Application();

    void run();

private:
    void initialize();

    void render();

    Window*  m_window;
};

#endif