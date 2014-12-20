#include <windows.h>

#include "Window/Mouse.h"
#include "Window/Window.h"


bool Mouse::isButtonPressed(Button button)
{
    return false;
}

glm::ivec2 Mouse::getPosition()
{
    POINT point;
    GetCursorPos(&point);
    return glm::ivec2(point.x, point.y);
}

glm::ivec2 Mouse::getPosition(const Window& window)
{
    HWND handle = window.getWindowHandle();
    if (handle != nullptr)
    {
        POINT point;
        GetCursorPos(&point);
        ScreenToClient(handle, &point);
        return glm::ivec2(point.x, point.y);
    }
    else
    {
        return glm::ivec2();
    }
}

void Mouse::setPosition(const glm::ivec2& position)
{
    SetCursorPos(position.x, position.y);
}

void Mouse::setPosition(const glm::ivec2& position, const Window& window)
{
    HWND handle = window.getWindowHandle();
    if (handle)
    {
        POINT point = { position.x, position.y};
        ClientToScreen(handle, &point);
        SetCursorPos(point.x, point.y);
    }
}