#include <windows.h>

#include "Input/Mouse.h"
#include "Window/Window.h"

namespace fsi
{
    bool Mouse::isButtonPressed(Button button)
    {
        int vkey = 0;
        switch (button)
        {
            case Mouse::Left:     vkey = VK_LBUTTON;  break;
            case Mouse::Right:    vkey = VK_RBUTTON;  break;
            case Mouse::Middle:   vkey = VK_MBUTTON;  break;
            case Mouse::XButton1: vkey = VK_XBUTTON1; break;
            case Mouse::XButton2: vkey = VK_XBUTTON2; break;
            default:              vkey = 0;           break;
        }

        return (GetAsyncKeyState(vkey) & 0x8000) != 0;
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
}