#pragma once

#include "glm/vec2.hpp"

namespace fsi
{
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

        static glm::ivec2 getPosition();
        static glm::ivec2 getPosition(const Window& relativeTo);

        static void setPosition(const glm::ivec2& position);
        static void setPosition(const glm::ivec2& position, const Window& relativeTo);
    };
}
