#pragma once

#include "Window/Keyboard.h"
#include "Window/Mouse.h"

struct Event
{
    // Keyboard event
    struct KeyEvent
    {
        Keyboard::Key code;
        bool          shift;
        bool          control;
        bool          alt;
    };

    // Mouse events
    struct MouseMoveEvent
    {
        int x;
        int y;
    };

    struct MouseButtonEvent
    {
        Mouse::Button button;
        int           x;
        int           y;
    };

    // Event used for resizing
    struct SizeEvent
    {
        int width;
        int height;
    };

    // Various event types
    enum EventType
    {
        Closed,
        KeyPressed,
        KeyReleased,
        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        MouseEntered,
        MouseLeft,
        Resized,

        Count
    };

    EventType type;

    union
    {
        KeyEvent             key;
        MouseMoveEvent       mouseMove;
        MouseButtonEvent     mouseButton;
        SizeEvent            size;
    };
};