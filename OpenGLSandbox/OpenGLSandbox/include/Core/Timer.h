#pragma once

#include <windows.h>

// Measures in microseconds

class Timer
{
public:
    Timer();
    ~Timer();

    __int64 getElapsedTime();
    __int64 restart();

private:
    LARGE_INTEGER getCurrentTime();

    LARGE_INTEGER m_accumulatedTime;
};