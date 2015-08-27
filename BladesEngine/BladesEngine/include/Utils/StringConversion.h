#pragma once

#include <sstream>
#include <iomanip>

namespace fsi
{
    template <class T>
    inline std::string to_string(const T& t, int prec = 2)
    {
        std::stringstream ss;
        ss << std::setprecision(prec) << std::fixed << t;
        return ss.str();
    }
}