#pragma once

#include <vector>

namespace fsi
{

// This class represents video mode used by a window
class VideoMode
{
public:
    // Default constructor, sets members to zero
    VideoMode();

    // Creates a video mode with given width, height and bits per pixel
    VideoMode(unsigned int modeWidth, unsigned int modeHeight, unsigned int modeBitsPerPixel);

    // Returns current desktop video mode of the device
    static VideoMode getDesktopVideoMode();

    // Returns all available fullscreen video modes
    static const std::vector<VideoMode> getFullScreenVideoModes();

    // Check whether current video mode is supported in full-screen
    bool isValidInFullScreen() const;

    unsigned int getModeWidth() const;
    void setModeWidth(unsigned int modeWidth);

    unsigned int getModeHeight() const;
    void setModeHeight(unsigned int modeHeight);

    unsigned int getModeBitsPerPixel() const;    
    void setModeBitsPerPixel(unsigned int modeBitsPerPixel);

private:
    unsigned int m_modeWidth;
    unsigned int m_modeHeight;
    unsigned int m_modeBitsPerPixel;
};

}
