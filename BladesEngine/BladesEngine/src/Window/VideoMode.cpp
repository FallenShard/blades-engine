#include <windows.h>

#include "Window/VideoMode.h"

namespace fsi
{

VideoMode::VideoMode()
    : m_width(0)
    , m_height(0)
    , m_bitsPerPixel(0)
{
}

VideoMode::VideoMode(unsigned int width, unsigned int height, unsigned int bitsPerPixel)
    : m_width(width)
    , m_height(height)
    , m_bitsPerPixel(bitsPerPixel)
{
}

VideoMode VideoMode::getDesktopVideoMode()
{
    // Windows-specific struct to obtain device mode
    DEVMODE win32DeviceMode;

    // As usual, most windows structures need to be informed of their size
    win32DeviceMode.dmSize = sizeof(win32DeviceMode);

    // Retrieves information about current video mode and puts them in the struct
    EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &win32DeviceMode);

    return VideoMode(win32DeviceMode.dmPelsWidth, win32DeviceMode.dmPelsHeight, win32DeviceMode.dmBitsPerPel);
}

const std::vector<VideoMode> VideoMode::getFullScreenVideoModes()
{
    std::vector<VideoMode> modes;

    // Windows-specific struct to obtain device mode
    DEVMODE win32DeviceMode;
    win32DeviceMode.dmSize = sizeof(win32DeviceMode);

    // Iterate through all the available device modes
    for (int count = 0; EnumDisplaySettings(nullptr, count, &win32DeviceMode); ++count)
        modes.emplace_back(win32DeviceMode.dmPelsWidth, win32DeviceMode.dmPelsHeight, win32DeviceMode.dmBitsPerPel);

    return modes;
}

bool VideoMode::isValidInFullScreen() const
{
    const std::vector<VideoMode>& fullScreenModes = getFullScreenVideoModes();

    bool validStatus = false;
    for (VideoMode mode : fullScreenModes)
    {
        if (m_width == mode.m_width && m_height == mode.m_height && m_bitsPerPixel == mode.m_bitsPerPixel)
        {
            validStatus = true;
            break;
        }
    }

    return validStatus;
}

unsigned int VideoMode::getWidth() const
{
    return m_width;
}

void VideoMode::setWidth(unsigned int width)
{
    m_width = width;
}

unsigned int VideoMode::getHeight() const
{
    return m_height;
}

void VideoMode::setHeight(unsigned int height)
{
    m_height = height;
}

unsigned int VideoMode::getBitsPerPixel() const
{
    return m_bitsPerPixel;
}

void VideoMode::setBitsPerPixel(unsigned int bitsPerPixel)
{
    m_bitsPerPixel = bitsPerPixel;
}

}