#include <fstream>
#include <iostream>
#include "Renderer/Shader.h"
#include "Utils/Logger.h"

Shader::Shader()
{
}

Shader::Shader(Shader::Type type)
{
    setShaderType(type);
}

Shader::Shader(std::string fileName, Shader::Type type)
{
    loadFromFile(fileName, type);
}

Shader::~Shader()
{
    glDeleteShader(m_identifier);
}

void Shader::loadFromFile(std::string fileName, Shader::Type type)
{
    // If the user specifies the optional type parameter
    if (type != None)
        setShaderType(type);

    // Read the shader source from the file
    m_sourceFileName = fileName;
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

    // Attach the source to a corresponding identifier
    const GLchar* source = m_source.c_str();
    glShaderSource(m_identifier, 1, &source, nullptr);

    // Compile the shader
    compile();
}

void Shader::compile()
{
    glCompileShader(m_identifier);
}

bool Shader::checkCompileStatus()
{
    GLint compileStatus = 0;

    // Assume that there was no error
    bool hasError = 0;

    // Get compilation status of a shader
    glGetShaderiv(m_identifier, GL_COMPILE_STATUS, &compileStatus);

    // If GL_FALSE was written in compileStatus, there was an error with the shader
    if (compileStatus == GL_FALSE)
    {
        hasError = true;
        int infoLogLength = 0;

        // Log the additional info
        glGetShaderiv(m_identifier, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar* buffer = new GLchar[infoLogLength];
        GLsizei charsWritten = 0;
        glGetShaderInfoLog(m_identifier, infoLogLength, &charsWritten, buffer);
        
        Logger::log(buffer);
        
        delete[] buffer;
    }

    return hasError;
}

void Shader::setShaderType(Shader::Type type)
{
    switch (type)
    {
    case Vert:        m_type = GL_VERTEX_SHADER;          break;
    case Geom:        m_type = GL_GEOMETRY_SHADER;        break;
    case TessControl: m_type = GL_TESS_CONTROL_SHADER;    break;
    case TessEval:    m_type = GL_TESS_EVALUATION_SHADER; break;
    case Frag:        m_type = GL_FRAGMENT_SHADER;        break;
    default:          m_type = GL_NONE;                   break;
    }

    if (m_type != GL_NONE)
        m_identifier = glCreateShader(m_type);
}

GLuint Shader::getShaderId() const
{
    return m_identifier;
}

std::string Shader::getSourceFileName() const
{
    return m_sourceFileName;
}