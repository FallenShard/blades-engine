#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <map>
#include <memory>
#include "Renderer/Shader.h"



class ShaderProgram
{
public:
    ShaderProgram();
    ~ShaderProgram();

    void attachShader(Shader& shader);

    void link();

    bool checkLinkStatus();

    GLuint getProgramId() const;

private:
    GLuint m_identifier;

    std::map<std::string, Shader*> m_shaders;
};


#endif // SHADER_PROGRAM_H