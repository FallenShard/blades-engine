#include "OglWrapper/Texture.h"

Texture::Texture()
{
    glGenTextures(1, &m_id);
}

Texture::Texture(int width, int height, std::string glslName)
    : m_size(width, height)
    , m_glslName(glslName)
    , m_textureUnit(0)
{
    glGenTextures(1, &m_id);
}

Texture::~Texture()
{
}

void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture::release()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::create(int mipmapLevel)
{
    glTexImage2D(GL_TEXTURE_2D, mipmapLevel, GL_RGB, m_size.x, m_size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
}

void Texture::setTextureUnit(GLint textureUnit)
{
    m_textureUnit = textureUnit;
}

void Texture::setMinFilter(GLint filter)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
}
void Texture::setMagFilter(GLint filter)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
}
void Texture::setWrapMode(GLint xDir, GLint yDir)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, xDir);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, yDir);
}

void Texture::locate(ShaderProgram* program)
{
    m_location = program->getUniformAttribute(m_glslName);
}

void Texture::activate()
{
    glActiveTexture(GL_TEXTURE0 + m_textureUnit);
}

void Texture::setSampler(ShaderProgram* program)
{
    program->setUniformSampler(m_location, GL_TEXTURE0 + m_textureUnit);
}

GLuint Texture::getTextureId() const
{
    return m_id;
}

GLint Texture::getSamplerLocation() const
{
    return m_location;
}