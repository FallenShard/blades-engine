#pragma once

#include <map>
#include "OpenGL.h"

namespace fsi
{

class ShaderProgram;

class TextureManager
{
public:
    struct TexInfo
    {
        GLuint id;
        GLuint unit;
        GLuint sampler;
        GLuint location;
    };

    enum SamplerPreset
    {
        LinearClamp  = 0,
        LinearRepeat = 1,

        Count
    };

    TextureManager();
    ~TextureManager();

    GLuint getSamplerPreset(SamplerPreset sampler) const;
    GLuint createSampler(GLenum minFilter, GLenum magFilter, GLenum sWrap, GLenum tWrap);

    TexInfo createTextureInfo(GLuint texId, GLuint sampler, ShaderProgram* program, std::string&& name);

    GLuint loadTexture(std::string&& fileName, GLsizei levels, GLenum internalFormat, GLenum format);

private:
    std::map<GLuint, TexInfo> m_textureCache;

    GLuint m_defaultSamplers[SamplerPreset::Count];

    int m_texUnitCount;



};

}