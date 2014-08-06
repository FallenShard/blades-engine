#ifndef SHADER_H
#define SHADER_H

#include <string>
#include "OpenGL.h"


class Shader
{
public:
    enum Type
    {
        Vert,
        Geom,
        TessControl,
        TessEval,
        Frag,
        None
    };

    Shader();
    Shader(Type type);

    ~Shader();

    void create(Type type);

    void loadFromFile(std::string fileName, Type type = None);
    void loadFromString(GLchar source[]);

    GLuint getShaderIdentifier() const;

    bool checkCompileError();

private:
    void setShaderType(Type type);
    void compile();

    std::string m_source;
    GLuint m_identifier;
    GLenum m_shaderType;
};


#endif // SHADER_H