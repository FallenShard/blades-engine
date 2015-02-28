#include <fstream>
#include <iostream>
#include <vector>
#include "OglWrapper/Shader.h"
#include "Utils/Logger.h"

namespace fsi
{

Shader::Shader()
{
}

Shader::Shader(Shader::Type type)
{
    // Shader type set through the constructor
    create(type);
}

Shader::Shader(std::string fileName, Shader::Type type)
{
    create(type);
    loadFromFile(fileName);
    compile();
}

Shader::~Shader()
{
    glDeleteShader(m_id);
}

bool Shader::create(Shader::Type type)
{
    switch (type)
    {
    case Vert:        m_type = GL_VERTEX_SHADER;          break;
    case Frag:        m_type = GL_FRAGMENT_SHADER;        break;
    case TessControl: m_type = GL_TESS_CONTROL_SHADER;    break;
    case TessEval:    m_type = GL_TESS_EVALUATION_SHADER; break;
    case Geom:        m_type = GL_GEOMETRY_SHADER;        break;
    default:          m_type = GL_NONE;                   break;
    }

    if (m_type != GL_NONE)
    {
        m_id = glCreateShader(m_type);
        return true;
    }
    else
        return false;
}

void Shader::loadFromFile(std::string fileName)
{
    // Read the shader source from the file
    m_sourceFileName = fileName;
    std::ifstream inputFile("res/" + fileName);

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
    glShaderSource(m_id, 1, &source, nullptr);
}

void Shader::compile()
{
    // Compiles the shader with a valid ID
    glCompileShader(m_id);
}

bool Shader::checkForErrors()
{
    GLint compileStatus = 0;

    // Assume that there was no error
    bool hasError = 0;

    // Get compilation status of a shader
    glGetShaderiv(m_id, GL_COMPILE_STATUS, &compileStatus);

    // If GL_FALSE was written in compileStatus, there was an error with the shader
    if (compileStatus == GL_FALSE)
    {
        hasError = true;
        
        // Get the length of additional info
        int infoLogLength = 0;
        glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &infoLogLength);

        std::vector<GLchar> buffer(infoLogLength);
        buffer.push_back('\0');
        GLsizei charsWritten = 0;
        glGetShaderInfoLog(m_id, infoLogLength, &charsWritten, &buffer[0]);

        std::string infoLog(buffer.begin(), buffer.end());
        
        LOG(m_sourceFileName + ": " + infoLog)
    }

    return hasError;
}

GLuint Shader::getShaderId() const
{
    return m_id;
}

std::string Shader::getSourceFileName() const
{
    return m_sourceFileName;
}

}