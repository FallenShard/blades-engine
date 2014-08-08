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
    Shader(std::string fileName, Type type);

    ~Shader();

    void create(Type type);

    void loadFromFile(std::string fileName, Type type = None);
    void compile();

    GLuint getShaderId() const;
    std::string getSourceFileName() const;

    bool checkCompileStatus();

private:
    void setShaderType(Type type);

    std::string m_sourceFileName;
    std::string m_source;
    GLuint m_identifier;
    GLenum m_type;
};


#endif // SHADER_H