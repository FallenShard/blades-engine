#include "Window/Mouse.h"

bool Mouse::isButtonPressed(Button button)
{
    return false;
}

Vector2Di Mouse::getPosition()
{
    return Vector2Di(0, 0);
}

Vector2Di Mouse::getPosition(const Window& relativeTo)
{
    return Vector2Di(0, 0);
}

void Mouse::setPosition(const Vector2Di& position)
{

}

void Mouse::setPosition(const Vector2Di& position, const Window& relativeTo)
{

}