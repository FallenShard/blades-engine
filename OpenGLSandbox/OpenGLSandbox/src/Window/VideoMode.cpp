#include <windows.h>

#include "Window/VideoMode.h"

VideoMode::VideoMode()
    :   m_modeWidth(0)
    ,   m_modeHeight(0)
    ,   m_modeBitsPerPixel(0)
{
}

VideoMode::VideoMode(unsigned int modeWidth, unsigned int modeHeight, unsigned int modeBitsPerPixel)
    :   m_modeWidth(modeWidth)
    ,   m_modeHeight(modeHeight)
    ,   m_modeBitsPerPixel(modeBitsPerPixel)
{
}

VideoMode VideoMode::getDesktopVideoMode()
{
    // This implementation is OS-specific

    // Windows-specific struct to obtain device mode
    DEVMODE win32DeviceMode;

    // As usual, most windows structures need to be informed of their size
    win32DeviceMode.dmSize = sizeof(win32DeviceMode);

    // Retrieves information about current video mode and puts them in the struct
    EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &win32DeviceMode);

    return VideoMode(win32DeviceMode.dmPelsWidth, win32DeviceMode.dmPelsHeight, win32DeviceMode.dmBitsPerPel);
}

const std::vector<VideoMode> VideoMode::getFullScreenVideoModes()
{
    // This implementation is OS-specific

    std::vector<VideoMode> modes;

    // Windows-specific struct to obtain device mode
    DEVMODE win32DeviceMode;
    win32DeviceMode.dmSize = sizeof(win32DeviceMode);

    // Iterate all the available device modes
    for (int count = 0; EnumDisplaySettings(NULL, count, &win32DeviceMode); ++count)
    {
        // Build a video mode
        VideoMode mode(win32DeviceMode.dmPelsWidth, win32DeviceMode.dmPelsHeight, win32DeviceMode.dmBitsPerPel);

        // Insert items that are not in the set already
        modes.push_back(mode);
    }

    return modes;
}

bool VideoMode::isValidInFullScreen() const
{
    const std::vector<VideoMode>& fullScreenModes = getFullScreenVideoModes();

    // Iterate through available modes and check whether the current one is valid for full-screen
    bool validStatus = false;
    for (VideoMode mode : fullScreenModes)
    {
        if (m_modeWidth == mode.m_modeWidth &&
            m_modeHeight == mode.m_modeHeight &&
            m_modeBitsPerPixel == mode.m_modeBitsPerPixel)
        {
            validStatus = true;
            break;
        }
    }

    return validStatus;
}

unsigned int VideoMode::getModeWidth() const
{
    return m_modeWidth;
}

void VideoMode::setModeWidth(unsigned int modeWidth)
{
    m_modeWidth = modeWidth;
}

unsigned int VideoMode::getModeHeight() const
{
    return m_modeHeight;
}

void VideoMode::setModeHeight(unsigned int modeHeight)
{
    m_modeHeight = modeHeight;
}

unsigned int VideoMode::getModeBitsPerPixel() const
{
    return m_modeBitsPerPixel;
}

void VideoMode::setModeBitsPerPixel(unsigned int modeBitsPerPixel)
{
    m_modeBitsPerPixel = modeBitsPerPixel;
}