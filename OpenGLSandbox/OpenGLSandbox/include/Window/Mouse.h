#pragma once

#include "Utils/Vector2D.h"

class Window;

class Mouse
{
public:
    enum Button
    {
        Left,
        Right,
        Middle,
        XButton1,
        XButton2,

        ButtonCount
    };

    static bool isButtonPressed(Button button);

    static Vector2Di getPosition();
    static Vector2Di getPosition(const Window& relativeTo);

    static void setPosition(const Vector2Di& position);
    static void setPosition(const Vector2Di& position, const Window& relativeTo);
};
