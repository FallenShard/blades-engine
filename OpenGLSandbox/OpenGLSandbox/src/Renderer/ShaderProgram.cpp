#include "Renderer/ShaderProgram.h"
#include "Utils/Logger.h"

ShaderProgram::ShaderProgram()
{
    // Create an identifier to recognize this program among others
    m_identifier = glCreateProgram();
}

void ShaderProgram::attachShader(Shader& shader)
{
    // Attach the shader to this program
    glAttachShader(m_identifier, shader.getShaderId());
    
    // Create and insert shader map element
    std::pair<std::string, Shader*> attachedShader;
    attachedShader.first = shader.getSourceFileName();
    attachedShader.second = &shader;
    m_shaders.insert(attachedShader);
}

void ShaderProgram::link()
{
    // Link the program
    glLinkProgram(m_identifier);
}

bool ShaderProgram::checkLinkStatus()
{
    // Used to grab actual error status
    GLint errorStatus = 0;

    // Assume that there was no error
    bool hasError = 0;

    // Get linking status of a program
    glGetProgramiv(m_identifier, GL_LINK_STATUS, &errorStatus);

    // If GL_FALSE was written in errorStatus, there was an error with linking
    if (errorStatus == GL_FALSE)
    {
        hasError = true;
        int infoLogLength = 0;

        // Log the additional info
        glGetProgramiv(m_identifier, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar* buffer = new GLchar[infoLogLength];
        GLsizei charsWritten = 0;
        glGetProgramInfoLog(m_identifier, infoLogLength, &charsWritten, buffer);

        Logger::getInstance()->log(buffer);

        delete[] buffer;
    }

    return hasError;
}

ShaderProgram::~ShaderProgram()
{
    m_shaders.clear();
}

GLuint ShaderProgram::getProgramId() const
{
    return m_identifier;
}