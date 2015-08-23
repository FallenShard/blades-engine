#include "Renderer/TextureManager.h"
#include "Renderer/Technique.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

namespace fsi
{
    const std::string TextureManager::relativePath = "res/textures/";

    TextureManager::TextureManager()
    {
        GLint maxTextureUnits;
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
        m_textureUnits.resize(maxTextureUnits);
        for (TextureUnit unit = 0; unit < static_cast<unsigned int>(maxTextureUnits); unit++)
            m_freeTextureUnits.insert(unit);

        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        m_defaultSamplers[LinearClamp] = createSampler(Filter::Linear, Filter::Linear, WrapMode::ClampToEdge, WrapMode::ClampToEdge);
        m_defaultSamplers[LinearRepeat] = createSampler(Filter::Linear, Filter::Linear, WrapMode::Repeat, WrapMode::Repeat);
        m_defaultSamplers[LinearMirrored] = createSampler(Filter::Linear, Filter::Linear, WrapMode::Mirrored, WrapMode::Mirrored);
        m_defaultSamplers[NearestClamp] = createSampler(Filter::Nearest, Filter::Nearest, WrapMode::ClampToEdge, WrapMode::ClampToEdge);
    }

    TextureManager::~TextureManager()
    {
        for (auto texId : m_textures)
            glDeleteTextures(1, &texId.first);
        m_textures.clear();

        glDeleteSamplers(m_samplers.size(), &m_samplers[0]);
        m_samplers.clear();

        glDeleteSamplers(m_defaultSamplers.size(), m_defaultSamplers.data());
    }

    SamplerHandle TextureManager::getSamplerPreset(SamplerPreset sampler) const
    {
        return m_defaultSamplers[sampler];
    }

    SamplerHandle TextureManager::createSampler(Filter minFilter, Filter magFilter, WrapMode sWrap, WrapMode tWrap)
    {
        GLuint sampler;
        glCreateSamplers(1, &sampler);
        glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, findGlFilter(minFilter));
        glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, findGlFilter(magFilter));
        glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, findGlWrapMode(sWrap));
        glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, findGlWrapMode(tWrap));

        m_samplers.push_back(sampler);

        return sampler;
    }

    TextureHandle TextureManager::loadTexture(const std::string& fileName, unsigned int levels, InternalFormat internalFormat, BaseFormat format)
    {
        GLenum internalGlFormat = findInternalGlFormat(internalFormat);
        GLenum baseGlFormat = findBaseGlFormat(format);
        int stbFormat = findStbFormat(format);
        int width, height, components;
        std::string fullFileName = relativePath + fileName;
        unsigned char* imgData = stbi_load(fullFileName.c_str(), &width, &height, &components, stbFormat);

        GLuint texId;
        glCreateTextures(GL_TEXTURE_2D, 1, &texId);
        glTextureStorage2D(texId, levels, internalGlFormat, width, height);
        glTextureSubImage2D(texId, 0, 0, 0, width, height, baseGlFormat, GL_UNSIGNED_BYTE, imgData);
        glGenerateTextureMipmap(texId);

        m_textures.emplace(texId, 0);

        stbi_image_free(imgData);

        return texId;
    }

    TextureHandle TextureManager::createTexture(unsigned int levels, unsigned int width, unsigned int height, InternalFormat internalFormat)
    {
        GLuint texId;
        glCreateTextures(GL_TEXTURE_2D, 1, &texId);
        glTextureStorage2D(texId, levels, findInternalGlFormat(internalFormat), width, height);
        glGenerateTextureMipmap(texId);

        m_textures.emplace(texId, 0);

        return texId;
    }

    TextureHandle TextureManager::load3DTexture(const std::vector<std::string>& fileNames, unsigned int levels, InternalFormat internalFormat, BaseFormat format)
    {
        GLenum internalGlFormat = findInternalGlFormat(internalFormat);
        GLenum baseGlFormat = findBaseGlFormat(format);
        int stbFormat = findStbFormat(format);
        int width, height, components;
        int depth = fileNames.size();

        std::vector<unsigned char*> images(fileNames.size());
        for (size_t i = 0; i < fileNames.size(); i++)
        {
            std::string fullFileName = relativePath + fileNames[i];
            images[i] = stbi_load(fullFileName.c_str(), &width, &height, &components, stbFormat);
        }

        GLuint texId;
        glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &texId);
        glTextureStorage3D(texId, levels, internalGlFormat, width, height, depth);

        for (int i = 0; i < depth; i++)
        {
            glTextureSubImage3D(texId, 0, 0, 0, i, width, height, 1, baseGlFormat, GL_UNSIGNED_BYTE, images[i]);
            stbi_image_free(images[i]);
        }
        glGenerateTextureMipmap(texId);

        m_textures.emplace(texId, 0);

        return texId;
    }

    void TextureManager::deleteTexture(TextureHandle texture)
    {
        glDeleteTextures(1, &texture);
        auto texUnitIter = m_textures.find(texture);
        if (texUnitIter != m_textures.end())
        {
            m_freeTextureUnits.insert(texUnitIter->second);
            m_textures.erase(texture);
        }
    }

    TextureManager::TexInfo TextureManager::createTextureInfo(TextureHandle texId, SamplerHandle sampler)
    {
        auto freeTexUnit = *(m_freeTextureUnits.begin());

        TexInfo texInfo;
        texInfo.id = texId;
        texInfo.sampler = sampler;
        texInfo.unit = freeTexUnit;

        glBindTextureUnit(texInfo.unit, texInfo.id);
        glBindSampler(texInfo.unit, texInfo.sampler);

        m_textures[texInfo.id] = texInfo.unit;
        m_textureUnits[texInfo.unit] = texInfo;
        m_freeTextureUnits.erase(freeTexUnit);

        return texInfo;
    }

    int TextureManager::findStbFormat(BaseFormat format)
    {
        switch (format)
        {
        case BaseFormat::Red:  return STBI_grey;
        case BaseFormat::RGB:  return STBI_rgb;
        case BaseFormat::RGBA: return STBI_rgb_alpha;
        default:               return STBI_rgb_alpha;
        }
    }

    GLenum TextureManager::findInternalGlFormat(InternalFormat format)
    {
        switch (format)
        {
        case InternalFormat::R8:    return GL_R8;
        case InternalFormat::RGB8:  return GL_RGB8;
        case InternalFormat::RGBA8: return GL_RGBA8;
        default:                    return GL_RGBA8;
        }
    }

    GLenum TextureManager::findBaseGlFormat(BaseFormat format)
    {
        switch (format)
        {
        case BaseFormat::Red:  return GL_RED;
        case BaseFormat::RGB:  return GL_RGB;
        case BaseFormat::RGBA: return GL_RGBA;
        default:               return GL_RGBA;
        }
    }
    
    GLenum TextureManager::findGlFilter(Filter filter)
    {
        switch (filter)
        {
        case Filter::Linear:  return GL_LINEAR;
        case Filter::Nearest: return GL_NEAREST;
        case Filter::Mipmap:  return GL_LINEAR_MIPMAP_LINEAR;
        default:              return GL_LINEAR;
        }
    }

    GLenum TextureManager::findGlWrapMode(WrapMode mode)
    {
        switch (mode)
        {
        case WrapMode::Repeat:        return GL_REPEAT;
        case WrapMode::ClampToBorder: return GL_CLAMP_TO_BORDER;
        case WrapMode::ClampToEdge:   return GL_CLAMP_TO_EDGE;
        case WrapMode::Mirrored:      return GL_MIRRORED_REPEAT;
        default:                      return GL_CLAMP_TO_EDGE;
        }
    }
}