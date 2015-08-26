#include <algorithm>

#include "Renderer/GLRenderer.h"
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
    Font::Font(GLRenderer* renderer, int pixelSize)
        : m_library(nullptr)
        , m_typeface(nullptr)
        , m_atlasWidth(0)
        , m_atlasHeight(0)
        , m_atlasTex(0)
    {
        if (!initFTLibrary())
            return;

        setTypeface(std::string("res/fonts/Calibri Bold.ttf"));
        setPixelSize(pixelSize);

        initFontAtlasDims();
        createAtlasTexture(renderer);
    }

    Font::~Font()
    {
        FT_Done_Face(m_typeface);
        FT_Done_FreeType(m_library);
    }

    void Font::setTypeface(std::string& fontFile)
    {
        if (FT_New_Face(m_library, fontFile.c_str(), 0, &m_typeface))
            LOG("Could not open font! Font file path: " + fontFile);
    }

    void Font::setPixelSize(int pixelSize)
    {
        FT_Set_Pixel_Sizes(m_typeface, 0, pixelSize);
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
        FT_GlyphSlot glyph = m_typeface->glyph;

        m_atlasHeight = 0;
        m_atlasWidth = 0;

        for (int i = 0; i < 128; i++)
        {
            if (FT_Load_Char(m_typeface, i, FT_LOAD_RENDER))
            {
                LOG("Loading character failed!");
                continue;
            }

            m_atlasWidth += glyph->bitmap.width;
            m_atlasHeight = std::max(m_atlasHeight, glyph->bitmap.rows);
        }
    }

    void Font::createAtlasTexture(GLRenderer* renderer)
    {
        auto texMgr = renderer->getTextureManager();
        m_atlasTex = texMgr->createTexture(1, m_atlasWidth, m_atlasHeight, InternalFormat::R8);

        FT_GlyphSlot g = m_typeface->glyph;
        int currX = 0;

        for (int i = 0; i < 128; i++)
        {
            if (FT_Load_Char(m_typeface, i, FT_LOAD_RENDER))
            {
                LOG("Loading character failed!");
                continue;
            }

            texMgr->updateTexture(m_atlasTex, 0, currX, 0, g->bitmap.width, g->bitmap.rows, BaseFormat::Red, g->bitmap.buffer);

            m_glyphCache[i].atlasOffsetX = static_cast<float>(currX) / static_cast<float>(m_atlasWidth);
            m_glyphCache[i].advanceX = (float)(g->advance.x >> 6);
            m_glyphCache[i].advanceY = (float)(g->advance.y >> 6);
            m_glyphCache[i].bmpWidth = (float)(g->bitmap.width);
            m_glyphCache[i].bmpHeight = (float)(g->bitmap.rows);
            m_glyphCache[i].bmpLeft = (float)(g->bitmap_left);
            m_glyphCache[i].bmpTop = (float)(g->bitmap_top);

            currX += g->bitmap.width;
        }

        auto sampler = texMgr->getSamplerPreset(TextureManager::LinearClamp);
        auto texInfo = texMgr->createTextureInfo(m_atlasTex, sampler);

        auto technique = std::make_unique<Technique>(renderer->getTechniqueCache()->getProgram("text"));
        technique->setUniformAttribute("atlas", texInfo.unit);
    }

    void Font::flipBitmapY(unsigned char* data, int width, int height)
    {
        for (int y = 0; y < height / 2; y++)
            for (int x = 0; x < width; x++)
                std::swap(data[y * width + x], data[(height - 1 - y) * width + x]);
    }
}