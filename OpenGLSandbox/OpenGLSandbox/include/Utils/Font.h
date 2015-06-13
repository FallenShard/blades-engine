#pragma once

#include <vector>

#include "FreeType/ft2build.h"
#include FT_FREETYPE_H

#include "OglWrapper/ShaderProgram.h"

namespace fsi
{

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

    Font(ShaderProgram* program);
    ~Font();

    void setTypeface(std::string& typeface);
    void setPixelSize(int pixelSize);

    GLuint prepareText(const char* text, float x, float y, float sx, float sy);

    const GlyphInfo& getGlyphInfo(char character) const;

    int getAtlasWidth() const;
    int getAtlasHeight() const;
    GLuint getAtlasTexId() const;

private:
    bool initFTLibrary();

    void initFontAtlasDims();
    void createAtlasTexture();

    void flipBitmapY(unsigned char* data, int w, int h);

    FT_Library m_library;
    FT_Face m_typeFace;

    GlyphInfo m_glyphCache[128];

    int m_atlasWidth;
    int m_atlasHeight;
    GLuint m_atlasTex;
    GLuint m_sampler;

    ShaderProgram* m_shaderProgram;
};

}