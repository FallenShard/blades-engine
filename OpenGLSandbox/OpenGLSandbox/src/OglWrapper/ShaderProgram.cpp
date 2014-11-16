#include "OglWrapper/ShaderProgram.h"
#include "Utils/Logger.h"

namespace
{
    GLuint prevBoundProgram = 0;
}

ShaderProgram::ShaderProgram()
{
    // Create an identifier to recognize this program among others
    m_id = glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
    m_shaders.clear();
}

void ShaderProgram::attachShader(Shader& shader)
{
    // Attach the shader to this program
    glAttachShader(m_id, shader.getShaderId());
    
    // Add the shader to the map with its filename as key
    m_shaders[shader.getSourceFileName()] = std::make_shared<Shader>(shader);
}

void ShaderProgram::attachShader(Shader::Type type, std::string sourceFileName)
{
    // Create a shader first
    Shader shader(sourceFileName, type);
    shader.checkCompileStatus();

    // Attach it to this program
    glAttachShader(m_id, shader.getShaderId());

    // Add the shader to the map
    m_shaders[shader.getSourceFileName()] = std::make_shared<Shader>(shader);
}

void ShaderProgram::detachShader(Shader& shader)
{
    // Detach the shader from this program
    glDetachShader(m_id, shader.getShaderId());

    // Remove the shader from current map
    auto foundShader = m_shaders.find(shader.getSourceFileName());
    if (foundShader != m_shaders.end())
        m_shaders.erase(foundShader);
}

void ShaderProgram::compile()
{
    for (auto& shader : m_shaders)
    {
        shader.second->compile();
    }
}

void ShaderProgram::link()
{
    // Link the program
    glLinkProgram(m_id);

    // Detach all the shaders to free up GPU memory
    for (auto& shader : m_shaders)
    {
        glDetachShader(m_id, shader.second->getShaderId());
    }
}

void ShaderProgram::use()
{
    if (prevBoundProgram != m_id)
    {
        prevBoundProgram = m_id;
        glUseProgram(m_id);
    }
}

void ShaderProgram::release()
{
    if (prevBoundProgram != 0)
    {
        prevBoundProgram = 0;
        glUseProgram(0);
    }
}

bool ShaderProgram::checkLinkStatus()
{
    // Used to grab actual error status
    GLint linkStatus = 0;

    // Assume that there was no error
    bool hasError = 0;

    // Get linking status of a program
    glGetProgramiv(m_id, GL_LINK_STATUS, &linkStatus);

    // If GL_FALSE was written in linkStatus, there was an error with linking
    if (linkStatus == GL_FALSE)
    {
        hasError = true;
        int infoLogLength = 0;

        // Log the additional info
        glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar* buffer = new GLchar[infoLogLength];
        GLsizei charsWritten = 0;
        glGetProgramInfoLog(m_id, infoLogLength, &charsWritten, buffer);

        Logger::log(buffer);

        delete[] buffer;
    }

    return hasError;
}

GLint ShaderProgram::getUniformAttribute(std::string name)
{
    // Query the location from OpenGL
    GLint location = glGetUniformLocation(m_id, name.c_str());

    // Insert the name and location into attribute hash table
    m_uniformAttributes[name] = location;

    return location;
}

GLint ShaderProgram::getUniformBuffer(std::string name)
{
    //Query the location from OpenGL
    GLint location = glGetUniformBlockIndex(m_id, name.c_str());

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

void ShaderProgram::setUniformAttribute(std::string name, GLfloat x, GLfloat y, GLfloat z)
{
    glUniform3f(m_uniformAttributes[name], x, y, z);
}

void ShaderProgram::setUniformAttribute(std::string name, GLsizei count, GLboolean transpose, const GLfloat* values)
{
    glUniformMatrix4fv(m_uniformAttributes[name], count, transpose, values);
}

void ShaderProgram::setUniformAttribute(std::string name, const glm::mat4& mat)
{
    glUniformMatrix4fv(m_uniformAttributes[name], 1, GL_FALSE, glm::value_ptr(mat));
}

void ShaderProgram::setUniformAttribute(std::string name, const glm::mat3& mat)
{
    glUniformMatrix3fv(m_uniformAttributes[name], 1, GL_FALSE, glm::value_ptr(mat));
}

void ShaderProgram::setUniformAttribute(std::string name, const glm::vec3& vec)
{
    glUniform3fv(m_uniformAttributes[name], 1, glm::value_ptr(vec));
}

void ShaderProgram::setUniformAttribute(std::string name, const glm::vec4& vec)
{
    glUniform4fv(m_uniformAttributes[name], 1, glm::value_ptr(vec));
}

void ShaderProgram::setUniformSampler(std::string name, GLint textureUnit)
{
    glUniform1i(m_uniformAttributes[name], textureUnit);
}

void ShaderProgram::setUniformSampler(GLint location, GLint textureUnit)
{
    glUniform1i(location, textureUnit);
}

GLuint ShaderProgram::getProgramId() const
{
    return m_id;
}