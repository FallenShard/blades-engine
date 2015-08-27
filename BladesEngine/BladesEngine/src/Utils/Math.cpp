#include "Utils/Math.h"

namespace fsi
{
    bool isPointInRect(int x, int y, glm::vec2& base, glm::vec2& size)
    {
        if (x < base.x || x >(base.x + size.x) || y < base.y || y >(base.y + size.y))
            return false;
        return true;
    }

    bool isPointInRect(int x, int y, glm::vec4& bounds)
    {
        if (x < bounds.x || x > bounds.z || y < bounds.y || y > bounds.w)
            return false;
        return true;
    }
}