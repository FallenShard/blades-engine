#pragma once

#include <windows.h>

#include <string>
#include <queue>
#include <memory>

#include "glm/vec2.hpp"

#include "Window/VideoMode.h"

#include "Input/Keyboard.h"
#include "Input/Event.h"

namespace fsi
{
    class Window
    {
    public:
        // Common window styles
        enum Style
        {
            Popup = 0,
            TitleBar = 1,
            Resizable = 1 << 1,
            CloseControls = 1 << 2,
            Overlapped = 1 << 3,

            Default = TitleBar | Resizable | CloseControls,
            Borderless = Popup
        };

        // Creates a new window based on given video mode, title and style
        Window(VideoMode mode, const std::string& windowTitle, Style windowStyle = Style::Borderless);

        // Closes the window and releases all alocated resources
        virtual ~Window();

        bool isOpen() const;

        bool pollEvent(Event& event);
        bool waitEvent(Event& event);

        void display();

        void close();

        void showMouseCursor(bool showCursor);
        glm::ivec2 getSize() const;
        glm::ivec2 getPosition() const;
        HWND getWindowHandle() const;

    private:
        // Called only the first time to register type of window created
        void registerWindowClass();

        DWORD translateWindowStyle(Window::Style style);

        // OS-specific function for handling messages
        static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

        // Transforms Windows virtual keys into appropriate key events
        static Keyboard::Key VirtualKeyToKeyEvent(WPARAM wParam, LPARAM lParam);

        void processMessage(UINT message, WPARAM wParam, LPARAM lParam);
        void processIncomingMessages(bool blocking);
        bool popEvent(Event& event, bool blocking);
        void pushEvent(const Event& event);

        std::queue<Event> m_eventQueue;

        glm::ivec2 m_size;
        glm::ivec2 m_prevSize;

        HWND m_windowHandle;
        HDC m_deviceContext;

        bool m_isOpen;
        bool m_containsMouse;
        bool m_isResizing;
    };
}