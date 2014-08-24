#include "Renderer/ShaderProgram.h"
#include "Utils/Logger.h"

ShaderProgram::ShaderProgram()
{
    // Create an identifier to recognize this program among others
    m_identifier = glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
    m_shaders.clear();
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

void ShaderProgram::detachShader(Shader& shader)
{
    // Detach the shader from this program
    glDetachShader(m_identifier, shader.getShaderId());

    // Remove the shader from current map
    auto foundShader = m_shaders.find(shader.getSourceFileName());
    if (foundShader != m_shaders.end())
        m_shaders.erase(foundShader);
}

void ShaderProgram::compile()
{
    // Compile all attached shaders
    for (auto& shader : m_shaders)
    {
        shader.second->compile();
    }
}

void ShaderProgram::link()
{
    // Link the program
    glLinkProgram(m_identifier);
}

void ShaderProgram::use()
{
    glUseProgram(m_identifier);
}

void ShaderProgram::release()
{
    glUseProgram(0);
}

bool ShaderProgram::checkLinkStatus()
{
    // Used to grab actual error status
    GLint linkStatus = 0;

    // Assume that there was no error
    bool hasError = 0;

    // Get linking status of a program
    glGetProgramiv(m_identifier, GL_LINK_STATUS, &linkStatus);

    // If GL_FALSE was written in linkStatus, there was an error with linking
    if (linkStatus == GL_FALSE)
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

GLint ShaderProgram::getUniformAttribute(std::string name)
{
    // Query the location from OpenGL
    GLint location = glGetUniformLocation(m_identifier, name.c_str());

    // Insert the name and location into attribute hash table
    m_uniformAttributes[name] = location;

    return location;
}

void ShaderProgram::setUniformAttribute(std::string name, GLfloat value)
{
    glUniform1f(m_uniformAttributes[name], value);
}

void ShaderProgram::setUniformAttribute(std::string name, GLfloat x, GLfloat y)
{
    glUniform2f(m_uniformAttributes[name], x, y);
}

void ShaderProgram::setUniformAttribute(std::string name, GLsizei count, GLboolean transpose, const GLfloat* values)
{
    glUniformMatrix4fv(m_uniformAttributes[name], count, transpose, values);
}


GLuint ShaderProgram::getProgramId() const
{
    return m_identifier;
}