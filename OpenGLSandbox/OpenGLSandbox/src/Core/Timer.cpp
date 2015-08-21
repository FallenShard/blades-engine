#include "Core/Timer.h"

namespace
{
    LARGE_INTEGER getFrequency()
    {
        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);
        return frequency;
    }
}

namespace fsi
{
    Timer::Timer()
        : m_accumulatedTime(getCurrentTime())
    {
    }

    Timer::~Timer()
    {
    }

    LARGE_INTEGER Timer::getCurrentTime()
    {
        // Get the frequency of the performance counter
        static LARGE_INTEGER frequency = getFrequency();

        // Get the current time
        LARGE_INTEGER currentTime;
        QueryPerformanceCounter(&currentTime);

        // Return the current time as microseconds
        currentTime.QuadPart *= 1000000;
        currentTime.QuadPart /= frequency.QuadPart;
        return currentTime;
    }

    __int64 Timer::getElapsedTime()
    {
        LARGE_INTEGER currentTime = getCurrentTime();
        __int64 elapsedTime = currentTime.QuadPart - m_accumulatedTime.QuadPart;

        return elapsedTime;
    }

    __int64 Timer::restart()
    {
        LARGE_INTEGER currentTime = getCurrentTime();
        __int64 elapsedTime = currentTime.QuadPart - m_accumulatedTime.QuadPart;
        m_accumulatedTime = currentTime;

        return elapsedTime;
    }
}