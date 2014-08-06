#include <fstream>
#include <iostream>
#include "Renderer/Shader.h"

Shader::Shader()
{
}

Shader::Shader(Shader::Type type)
{
    setShaderType(type);
}

Shader::~Shader()
{
}

void Shader::loadFromFile(std::string fileName, Shader::Type type)
{
    // Open a stream to read source from
    std::ifstream inputFile(fileName);

    if (inputFile.is_open())
    {
        std::string line;
        while (std::getline(inputFile, line))
        {
            m_source += line + '\n';
        }
        inputFile.close();
    }

    const GLchar* source = m_source.c_str();
    glShaderSource(m_identifier, 1, &source, nullptr);

    compile();
}

void Shader::loadFromString(GLchar source[])
{

}

void Shader::compile()
{
    glCompileShader(m_identifier);
}

bool Shader::checkCompileError()
{
    GLint errorStatus = 0;

    // Assume that there was no error
    bool hasError = 0;

    // Get compilation status of a shader
    glGetShaderiv(m_identifier, GL_COMPILE_STATUS, &errorStatus);

    // If GL_FALSE was written in errorStatus, there was an error with the shader
    if (errorStatus == GL_FALSE)
    {
        hasError = true;
        int infoLogLength = 0;

        // Get addition info log
        glGetShaderiv(m_identifier, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar* buffer = new GLchar[infoLogLength];
        GLsizei charsWritten = 0;
        glGetShaderInfoLog(m_identifier, infoLogLength, &charsWritten, buffer);
        
        std::ofstream errorLog("errorLog.txt");
        errorLog << buffer;
        errorLog.close();

        delete buffer;
    }

    return hasError;
}

void Shader::setShaderType(Shader::Type type)
{
    switch (type)
    {
    case Vert:        m_shaderType = GL_VERTEX_SHADER;          break;
    case Geom:        m_shaderType = GL_GEOMETRY_SHADER;        break;
    case TessControl: m_shaderType = GL_TESS_CONTROL_SHADER;    break;
    case TessEval:    m_shaderType = GL_TESS_EVALUATION_SHADER; break;
    case Frag:        m_shaderType = GL_FRAGMENT_SHADER;        break;
    default:          m_shaderType = 0;                         break;
    }

    if (m_shaderType != 0)
        m_identifier = glCreateShader(m_shaderType);
}

GLuint Shader::getShaderIdentifier() const
{
    return m_identifier;
}