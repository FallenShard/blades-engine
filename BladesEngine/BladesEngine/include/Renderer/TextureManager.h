#pragma once

#include <map>
#include <vector>
#include <array>
#include <set>
#include "OpenGL.h"

namespace fsi
{
    using TextureHandle = GLuint;
    using SamplerHandle = GLuint;
    using TextureUnit = GLuint;

    enum class InternalFormat
    {
        R8,
        RGB8,
        RGBA8
    };

    enum class BaseFormat
    {
        Red,
        RGB,
        RGBA
    };

    enum class Filter
    {
        Linear,
        Nearest,
        Mipmap
    };

    enum class WrapMode
    {
        Repeat,
        ClampToEdge,
        ClampToBorder,
        Mirrored
    };

    class TextureManager
    {
    public:
        struct TexInfo
        {
            TextureHandle id;
            SamplerHandle sampler;
            TextureUnit unit;
        };

        enum SamplerPreset
        {
            LinearClamp = 0,
            LinearRepeat = 1,
            LinearMirrored = 2,
            NearestClamp = 3,
            Count
        };

        TextureManager();
        ~TextureManager();

        SamplerHandle getSamplerPreset(SamplerPreset sampler) const;
        SamplerHandle createSampler(Filter minFilter, Filter magFilter, WrapMode sWrap, WrapMode tWrap);

        TexInfo createTextureInfo(TextureHandle texture, SamplerHandle sampler);

        TextureHandle loadTexture(const std::string& fileName, unsigned int levels, InternalFormat internalFormat, BaseFormat format);
        TextureHandle createTexture(unsigned int levels, unsigned int width, unsigned int height, InternalFormat internalFormat);
        
        TextureHandle load3DTexture(const std::vector<std::string>& fileNames, unsigned int levels, InternalFormat internalFormat, BaseFormat format);

        void deleteTexture(TextureHandle texture);

    private:
        int findStbFormat(BaseFormat format);
        GLenum findInternalGlFormat(InternalFormat format);
        GLenum findBaseGlFormat(BaseFormat format);
        GLenum findGlFilter(Filter filter);
        GLenum findGlWrapMode(WrapMode mode);

        std::vector<TexInfo> m_textureUnits;
        std::set<TextureUnit> m_freeTextureUnits;

        std::map<TextureHandle, TextureUnit> m_textures;
        std::vector<SamplerHandle> m_samplers;

        std::array<SamplerHandle, Count> m_defaultSamplers;

        const static std::string relativePath;
    };
}