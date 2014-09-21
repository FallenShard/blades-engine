#pragma once

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

    bool create(Type type);

    void loadFromFile(std::string fileName, Type type = None);
    void compile();

    GLuint getShaderId() const;
    std::string getSourceFileName() const;

    bool checkCompileStatus();

private:
    std::string m_sourceFileName;
    std::string m_source;
    GLuint m_id;
    GLenum m_type;
};