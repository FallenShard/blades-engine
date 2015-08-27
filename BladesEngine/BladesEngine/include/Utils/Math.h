#pragma once

#include "glm/glm.hpp"

namespace fsi
{
    bool isPointInRect(int x, int y, glm::vec2& base, glm::vec2& size);
    bool isPointInRect(int x, int y, glm::vec4& bounds);
}