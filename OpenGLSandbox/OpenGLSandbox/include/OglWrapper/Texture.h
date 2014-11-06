#pragma once

#include "OpenGL.h"

#include "OglWrapper/ShaderProgram.h"

class Texture
{
public:
    Texture();
    Texture(int width, int height, std::string glslName);
    ~Texture();

    void bind();
    static void release();

    void create(int mipmapLevel = 0);

    void setTextureUnit(GLint textureUnit);
    void setMinFilter(GLint filter);
    void setMagFilter(GLint filter);
    void setWrapMode(GLint xDir, GLint yDir);
    
    void locate(ShaderProgram* program);
    void activate();
    void setSampler(ShaderProgram* program);

    GLuint getTextureId() const;
    GLint  getSamplerLocation() const;

private:
    GLuint m_id;

    glm::ivec2 m_size;
    
    std::string m_glslName;
    GLint m_location;
    GLint m_textureUnit;
};