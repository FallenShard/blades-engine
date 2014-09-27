#include <windows.h>

#include "Window/Mouse.h"
#include "Window/Window.h"


bool Mouse::isButtonPressed(Button button)
{
    return false;
}

Vector2Di Mouse::getPosition()
{
    POINT point;
    GetCursorPos(&point);
    return Vector2Di(point.x, point.y);
}

Vector2Di Mouse::getPosition(const Window& window)
{
    HWND handle = window.getWindowHandle();
    if (handle != nullptr)
    {
        POINT point;
        GetCursorPos(&point);
        ScreenToClient(handle, &point);
        return Vector2Di(point.x, point.y);
    }
    else
    {
        return Vector2Di();
    }
}

void Mouse::setPosition(const Vector2Di& position)
{

}

void Mouse::setPosition(const Vector2Di& position, const Window& relativeTo)
{

}