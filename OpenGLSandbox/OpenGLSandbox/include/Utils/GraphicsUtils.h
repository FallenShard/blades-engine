#ifndef GRAPHICSUTILS_H
#define GRAPHICSUTILS_H

namespace fi
{

typedef struct Color
{
    unsigned int    rgba;
    unsigned char   r, g, b, a;

    void uintToChar(unsigned int color)
    {
        r = color >> 24;
        g = (color >> 16) & 0xFF;
        b = (color >> 8) & 0xFF;
        a = color & 0xFF;
    }

    Color(unsigned char rr, unsigned char gg, unsigned char bb, unsigned char aa)
    {
        r = rr; g = gg; b = bb; a = aa;
    }

    Color(unsigned char rr, unsigned char gg, unsigned char bb)
    {
        r = rr; g = gg; b = bb; a = 255;
    }

    Color(unsigned int colorCode)
    {
        rgba = colorCode;
        uintToChar(colorCode);
    }

    
};

static const Color Blue(0x0000FFFF);
static const Color Red(0xFF0000FF);
static const Color Green(0x00FF00FF);

}
#endif