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
    void processInput();
    void render();
    void renderInit();

    Window*  m_window;
};

#endif