#include <algorithm>

#include "Renderer/VertexAssembly.h"
#include "Utils/Font.h"
#include "Utils/Logger.h"

namespace
{
    GLuint atlasUnifLoc = 0;
    GLuint atlasTexUnit = 20;

    glm::mat4 orthoProj = glm::ortho(0.f, 1360.f, 768.f, 0.f);

    GLuint vao;
    GLuint vbo;
}

namespace fsi
{
    Font::Font(Technique* program)
        : m_library(nullptr)
        , m_typeFace(nullptr)
        , m_shaderProgram(program)
    {
        if (!initFTLibrary())
            return;

        setTypeface(std::string("res/Calibri Bold.ttf"));
        setPixelSize(20);

        initFontAtlasDims();
        createAtlasTexture();
    }

    Font::~Font()
    {
        FT_Done_Face(m_typeFace);
        FT_Done_FreeType(m_library);
    }

    void Font::setTypeface(std::string& fontFile)
    {
        if (FT_New_Face(m_library, fontFile.c_str(), 0, &m_typeFace))
            LOG("Could not open font! Font file: " + fontFile);
    }

    void Font::setPixelSize(int pixelSize)
    {
        FT_Set_Pixel_Sizes(m_typeFace, 0, pixelSize);
    }

    const Font::GlyphInfo& Font::getGlyphInfo(char character) const
    {
        return m_glyphCache[character];
    }

    int Font::getAtlasHeight() const
    {
        return m_atlasHeight;
    }

    int Font::getAtlasWidth() const
    {
        return m_atlasWidth;
    }

    GLuint Font::getAtlasTexId() const
    {
        return m_atlasTex;
    }

    bool Font::initFTLibrary()
    {
        if (FT_Init_FreeType(&m_library))
        {
            LOG("Failed to initialize FreeType library!");
            return false;
        }

        return true;
    }

    void Font::initFontAtlasDims()
    {
        FT_GlyphSlot g = m_typeFace->glyph;

        m_atlasHeight = 0;
        m_atlasWidth = 0;

        for (int i = 0; i < 128; i++)
        {
            if (FT_Load_Char(m_typeFace, i, FT_LOAD_RENDER))
            {
                LOG("Loading character failed!");
                continue;
            }

            m_atlasWidth += g->bitmap.width;
            m_atlasHeight = std::max(m_atlasHeight, g->bitmap.rows);
        }
    }

    void Font::createAtlasTexture()
    {
        atlasUnifLoc = 0;// glGetUniformLocation(m_shaderProgram->getProgramId(), "atlas");
        m_shaderProgram->use();
        //m_shaderProgram->setUniformSampler(atlasUnifLoc, atlasTexUnit);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_atlasTex);
        glTextureStorage2D(m_atlasTex, 1, GL_R8, m_atlasWidth, m_atlasHeight);
        glGenerateTextureMipmap(m_atlasTex);

        FT_GlyphSlot g = m_typeFace->glyph;
        int x = 0;

        for (int i = 0; i < 128; i++)
        {
            if (FT_Load_Char(m_typeFace, i, FT_LOAD_RENDER))
            {
                LOG("Loading character failed!");
                continue;
            }

            glTextureSubImage2D(m_atlasTex, 0, x, 0, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

            m_glyphCache[i].atlasOffsetX = static_cast<float>(x) / static_cast<float>(m_atlasWidth);
            m_glyphCache[i].advanceX = (float)(g->advance.x >> 6);
            m_glyphCache[i].advanceY = (float)(g->advance.y >> 6);
            m_glyphCache[i].bmpWidth = (float)(g->bitmap.width);
            m_glyphCache[i].bmpHeight = (float)(g->bitmap.rows);
            m_glyphCache[i].bmpLeft = (float)(g->bitmap_left);
            m_glyphCache[i].bmpTop = (float)(g->bitmap_top);

            x += g->bitmap.width;
        }

        glCreateSamplers(1, &m_sampler);
        glSamplerParameteri(m_sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glSamplerParameteri(m_sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glBindTextureUnit(atlasTexUnit, m_atlasTex);
        glBindSampler(atlasTexUnit, m_sampler);
    }

    void Font::flipBitmapY(unsigned char* data, int width, int height)
    {
        for (int y = 0; y < height / 2; y++)
        {
            for (int x = 0; x < width; x++)
            {
                unsigned char temp = data[y * width + x];
                data[y * width + x] = data[(height - 1 - y) * width + x];
                data[(height - 1 - y) * width + x] = temp;
            }
        }
    }
}