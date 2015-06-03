#pragma once

#include <vector>

#include "FreeType/ft2build.h"
#include FT_FREETYPE_H

#include "OglWrapper/ShaderProgram.h"

namespace fsi
{

class TextRenderer
{
public:

    TextRenderer(ShaderProgram* program);
    ~TextRenderer();

    void setTypeface(std::string& typeface);
    void setPixelSize(int pixelSize);

    void prepareText(const char* text, float x, float y, float sx, float sy);
    void render();
    void resize(int width, int height);

private:
    bool initFTLibrary();

    void initFontAtlasDims();
    void createAtlasTexture();

    void flipBitmapY(unsigned char* data, int w, int h);

    FT_Library m_library;
    FT_Face m_typeFace;

    int m_atlasWidth;
    int m_atlasHeight;

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

    GlyphInfo m_glyphCache[128];

    ShaderProgram* m_shaderProgram;
    GLuint m_atlasTex;
    GLuint m_sampler;

    GLuint m_vao;
    GLuint m_vbo;

    std::vector<GLfloat> m_vertices;
};

}