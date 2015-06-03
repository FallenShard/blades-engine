#include <algorithm>

#include "OglWrapper/VertexStreams.h"
#include "Renderer/TextRenderer.h"
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

TextRenderer::TextRenderer(ShaderProgram* program)
    : m_library(nullptr)
    , m_typeFace(nullptr)
    , m_shaderProgram(program)
{

    if (!initFTLibrary())
        return;

    setTypeface(std::string("res/Calibri.ttf"));
    setPixelSize(20);
    
    initFontAtlasDims();
    createAtlasTexture();
}

TextRenderer::~TextRenderer()
{
    FT_Done_Face(m_typeFace);
    FT_Done_FreeType(m_library);
}

void TextRenderer::setTypeface(std::string& fontFile)
{
    if (FT_New_Face(m_library, fontFile.c_str(), 0, &m_typeFace))
        LOG("Could not open font! Font file: " + fontFile);
}

void TextRenderer::setPixelSize(int pixelSize)
{
    FT_Set_Pixel_Sizes(m_typeFace, 0, pixelSize);
}

void TextRenderer::prepareText(const char* text, float x, float y, float sx, float sy)
{
    m_vertices.reserve(24 * 1);
    int ind = 0;
    GlyphInfo* gInfo;

    for (const char *p = text; *p; p++)
    {
        gInfo = &m_glyphCache[*p];
        float x1 = (x + m_glyphCache[*p].bmpLeft * sx);
        float y1 = (y - (m_glyphCache[*p].bmpTop) * sy);
        float x2 = x1 + (m_glyphCache[*p].bmpWidth * sx);
        float y2 = y1 + (m_glyphCache[*p].bmpHeight * sy);

        /* Advance the cursor to the start of the next character */
        x += m_glyphCache[*p].advanceX * sx;
        y += m_glyphCache[*p].advanceY * sy;

        m_vertices.push_back(x1);
        m_vertices.push_back(y1);
        m_vertices.push_back(m_glyphCache[*p].atlasOffsetX);
        m_vertices.push_back(0.0f);

        m_vertices.push_back(x2);
        m_vertices.push_back(y2);
        m_vertices.push_back(m_glyphCache[*p].atlasOffsetX + m_glyphCache[*p].bmpWidth / (float)m_atlasWidth);
        m_vertices.push_back(m_glyphCache[*p].bmpHeight / (float)m_atlasHeight);

        m_vertices.push_back(x2);
        m_vertices.push_back(y1);
        m_vertices.push_back(m_glyphCache[*p].atlasOffsetX + m_glyphCache[*p].bmpWidth / (float)m_atlasWidth);
        m_vertices.push_back(0.0f);

        m_vertices.push_back(x1);
        m_vertices.push_back(y1);
        m_vertices.push_back(m_glyphCache[*p].atlasOffsetX);
        m_vertices.push_back(0.0f);

        m_vertices.push_back(x1);
        m_vertices.push_back(y2);
        m_vertices.push_back(m_glyphCache[*p].atlasOffsetX);
        m_vertices.push_back(m_glyphCache[*p].bmpHeight / (float)m_atlasHeight);

        m_vertices.push_back(x2);
        m_vertices.push_back(y2);
        m_vertices.push_back(m_glyphCache[*p].atlasOffsetX + m_glyphCache[*p].bmpWidth / (float)m_atlasWidth);
        m_vertices.push_back(m_glyphCache[*p].bmpHeight / (float)m_atlasHeight);

        
    }

    glCreateVertexArrays(1, &m_vao);
    glCreateBuffers(1, &m_vbo);

    glNamedBufferData(m_vbo, m_vertices.size() * sizeof(GLfloat), m_vertices.data(), GL_DYNAMIC_DRAW);

    glVertexArrayVertexBuffer(m_vao, VertexBufferBinding::Slot0, m_vbo, 0, 4 * sizeof(GLfloat));

    glVertexArrayAttribBinding(m_vao, VertexAttrib::Position, VertexBufferBinding::Slot0);
    glVertexArrayAttribFormat(m_vao, VertexAttrib::Position, 4, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(m_vao, VertexAttrib::Position);
}

void TextRenderer::render()
{
    glm::mat4 gridMvp(1.f);
    m_shaderProgram->use();
    m_shaderProgram->setUniformAttribute("MVP", gridMvp);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm::vec4 color = glm::vec4(1.f, 0.f, 0.f, 1.f);
    m_shaderProgram->setUniformAttribute("color", color);

    glm::mat4 modelMat = glm::translate(glm::vec3(200.f, 300.f, 0.f));
    m_shaderProgram->setUniformAttribute("MVP", orthoProj * modelMat);

    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size() / 4);
    
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}

void TextRenderer::resize(int width, int height)
{
    orthoProj = glm::ortho(0.f, static_cast<float>(width), static_cast<float>(height), 0.f);
}

bool TextRenderer::initFTLibrary()
{
    if (FT_Init_FreeType(&m_library))
    {
        LOG("Failed to initialize FreeType library!");
        return false;
    }

    return true;
}

void TextRenderer::initFontAtlasDims()
{
    FT_GlyphSlot g = m_typeFace->glyph;

    m_atlasHeight = 0;
    m_atlasWidth = 0;

    for (int i = 0x41; i < 0x80; i++)
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

void TextRenderer::createAtlasTexture()
{
    atlasUnifLoc = glGetUniformLocation(m_shaderProgram->getProgramId(), "atlas");
    m_shaderProgram->use();
    m_shaderProgram->setUniformSampler(atlasUnifLoc, atlasTexUnit);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glCreateTextures(GL_TEXTURE_2D, 1, &m_atlasTex);
    glTextureStorage2D(m_atlasTex, 1, GL_R8, m_atlasWidth, m_atlasHeight);
    glGenerateTextureMipmap(m_atlasTex);

    FT_GlyphSlot g = m_typeFace->glyph;
    int x = 0;

    for (int i = 0x41; i < 0x80; i++)
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

void TextRenderer::flipBitmapY(unsigned char* data, int width, int height)
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