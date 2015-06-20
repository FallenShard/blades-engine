#include "Renderer/TextureManager.h"
#include "OglWrapper/ShaderProgram.h"
#include "stb/stb_image.h"

namespace fsi
{

TextureManager::TextureManager()
    : m_texUnitCount(0)
{
    m_defaultSamplers[LinearClamp] = createSampler(GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    m_defaultSamplers[LinearRepeat] = createSampler(GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT);
}

TextureManager::~TextureManager()
{

}

GLuint TextureManager::getSamplerPreset(SamplerPreset sampler) const
{
    return m_defaultSamplers[sampler];
}

GLuint TextureManager::createSampler(GLenum minFilter, GLenum magFilter, GLenum sWrap, GLenum tWrap)
{
    GLuint sampler;
    glCreateSamplers(1, &sampler);
    glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, minFilter);
    glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, magFilter);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, sWrap);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, tWrap);

    return sampler;
}

GLuint TextureManager::loadTexture(std::string&& fileName, GLsizei levels, GLenum internalFormat, GLenum format)
{
    int w, h, comp;
    unsigned char* img = stbi_load("res/checkbox.png", &w, &h, &comp, STBI_rgb_alpha);

    GLuint texId;
    glCreateTextures(GL_TEXTURE_2D, 1, &texId);
    glTextureStorage2D(texId, levels, internalFormat, w, h);
    glTextureSubImage2D(texId, 0, 0, 0, w, h, format, GL_UNSIGNED_BYTE, img);
    glGenerateTextureMipmap(texId);

    return texId;
}

TextureManager::TexInfo TextureManager::createTextureInfo(GLuint texId, GLuint sampler, ShaderProgram* program, std::string&& name)
{
    TexInfo texInfo;
    texInfo.id = texId;
    texInfo.sampler = sampler;
    texInfo.location = glGetUniformLocation(program->getProgramId(), name.c_str());
    texInfo.unit = m_texUnitCount++;

    program->setUniformSampler(texInfo.location, texInfo.unit);
    glBindTextureUnit(texInfo.unit, texInfo.id);
    glBindSampler(texInfo.unit, texInfo.sampler);

    return texInfo;
}



}