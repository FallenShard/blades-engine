#ifndef WINDOW_H
#define WINDOW_H

#include <windows.h>

#include <string>
#include <queue>

#include "Utils/Vector2D.h"
#include "VideoMode.h"
#include "GLContext.h"

#include "Window/Keyboard.h"
#include "Window/Event.h"

// Basic window class for OpenGL rendering
class Window
{
public:
    // Common window styles
    enum Style
    {
        None = 0,
        TitleBar = 1,
        Resizable = 1 << 1,
        CloseControls = 1 << 2,

        Default = TitleBar | Resizable | CloseControls
    };

    // Creates a new window based on given video mode, title and style
    Window(VideoMode mode, const std::string& windowTitle, Style windowStyle = Style::Default);

    // Closes the window and releases all alocated resources
    virtual ~Window();

    bool isOpen() const;

    bool pollEvent(Event& event);

    void display();

    void close();

private:
    // Called only the first time to register type of window created
    void registerWindowClass();

    // OS-specific function for handling messages
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    // Transforms Windows virtual keys into appropriate key events
    static Keyboard::Key VirtualKeyToKeyEvent(WPARAM wParam, LPARAM lParam);

    void processMessage(UINT message, WPARAM wParam, LPARAM lParam);
    void processIncomingMessages(bool blocking);
    bool popEvent(Event& event, bool blocking);
    void pushEvent(const Event& event);

    std::queue<Event> m_eventQueue;

    Vector2Du m_size;
    HWND m_windowHandle;
    HDC m_deviceContextHandle;
    GLContext* m_GLContext;
    bool m_isOpen;
    bool m_containsMouse;
};

#endif // WINDOW_H