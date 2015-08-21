#pragma once

#include <vector>

namespace fsi
{
    class VideoMode
    {
    public:
        VideoMode();
        VideoMode(unsigned int width, unsigned int height, unsigned int bitsPerPixel);

        static VideoMode getDesktopVideoMode();
        static const std::vector<VideoMode> getFullScreenVideoModes();

        bool isValidInFullScreen() const;

        unsigned int getWidth() const;
        void setWidth(unsigned int modeWidth);

        unsigned int getHeight() const;
        void setHeight(unsigned int modeHeight);

        unsigned int getBitsPerPixel() const;    
        void setBitsPerPixel(unsigned int modeBitsPerPixel);

    private:
        unsigned int m_width;
        unsigned int m_height;
        unsigned int m_bitsPerPixel;
    };
}
