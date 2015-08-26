#pragma once

#include <vector>

#include "FreeType/ft2build.h"
#include FT_FREETYPE_H

#include "OpenGL.h"

namespace fsi
{
    class GLRenderer;

    class Font
    {
    public:
        struct GlyphInfo
        {
            float advanceX;
            float advanceY;

            float bmpHeight;
            float bmpWidth;

            float bmpLeft;
            float bmpTop;

            float atlasOffsetX;
        };

        Font(GLRenderer* renderer, int pixelSize = 20);
        ~Font();

        void setTypeface(std::string& typeface);
        void setPixelSize(int pixelSize);

        const GlyphInfo& getGlyphInfo(char character) const;

        int getAtlasWidth() const;
        int getAtlasHeight() const;
        GLuint getAtlasTexId() const;

    private:
        bool initFTLibrary();

        void initFontAtlasDims();
        void createAtlasTexture(GLRenderer* renderer);

        void flipBitmapY(unsigned char* data, int width, int height);

        FT_Library m_library;
        FT_Face m_typeface;

        GlyphInfo m_glyphCache[128];

        int m_atlasWidth;
        int m_atlasHeight;
        GLuint m_atlasTex;

    };
}