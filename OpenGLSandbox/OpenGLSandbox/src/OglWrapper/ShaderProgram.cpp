#include <vector>

#include "OglWrapper/ShaderProgram.h"
#include "Utils/Logger.h"

namespace
{
    GLuint prevBoundProgram = 0;
}

namespace fsi
{

ShaderProgram::ShaderProgram()
{
    // Create an identifier to recognize this program among others
    m_id = glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
    m_shaders.clear();

    glDeleteProgram(m_id);
}

void ShaderProgram::attachShader(Shader& shader)
{
    // Attach the shader to this program
    glAttachShader(m_id, shader.getShaderId());
    
    // Add the shader to the map with its filename as key
    m_shaders[shader.getSourceFileName()] = std::make_shared<Shader>(shader);
}

void ShaderProgram::attachShader(std::shared_ptr<Shader> shader)
{
    glAttachShader(m_id, shader->getShaderId());
    
    m_shaders[shader->getSourceFileName()] = shader;
}

void ShaderProgram::attachShader(Shader::Type type, std::string sourceFileName)
{
    // Create a shader first
    Shader shader(sourceFileName, type);
    shader.checkForErrors();

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

bool ShaderProgram::checkForErrors()
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

        // Get the length of additional info
        int infoLogLength = 0;
        glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &infoLogLength);
        
        std::vector<GLchar> buffer(infoLogLength);
        buffer.push_back('\0');
        GLsizei charsWritten = 0;
        glGetProgramInfoLog(m_id, infoLogLength, &charsWritten, &buffer[0]);

        std::string infoLog(buffer.begin(), buffer.end());

        LOG(infoLog)
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

void ShaderProgram::bindVertexAttribute(GLuint index, std::string name)
{
    glBindAttribLocation(m_id, index, name.c_str());
}

void ShaderProgram::setUniformAttribute(std::string name, GLfloat value)
{
    glUniform1f(m_uniformAttributes[name], value);
}

void ShaderProgram::setUniformAttribute(std::string name, GLint value)
{
    glUniform1i(m_uniformAttributes[name], value);
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
    const float* ptr = glm::value_ptr(mat);
    glUniformMatrix3fv(m_uniformAttributes[name], 1, GL_FALSE, glm::value_ptr(mat));
}

void ShaderProgram::setUniformAttribute(std::string name, const glm::vec2& vec)
{
    glUniform2fv(m_uniformAttributes[name], 1, glm::value_ptr(vec));
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

void ShaderProgram::setUniformBlockBinding(GLuint blockIndex, GLuint bindingPoint)
{
    glUniformBlockBinding(m_id, blockIndex, bindingPoint);
}

void ShaderProgram::setUniformBlockBinding(std::string blockName, GLuint bindingPoint)
{
    GLuint blockIndex = glGetUniformBlockIndex(m_id, blockName.c_str());
    glUniformBlockBinding(m_id, blockIndex, bindingPoint);
}

GLuint ShaderProgram::getProgramId() const
{
    return m_id;
}

void ShaderProgram::queryActiveUniforms()
{
    GLint numActiveUniforms = 0;
    glGetProgramiv(m_id, GL_ACTIVE_UNIFORMS, &numActiveUniforms);

    std::vector<GLchar> nameData(256);

    for (int unif = 0; unif < numActiveUniforms; ++unif)
    {
        GLint arraySize = 0;
        GLenum type = 0;
        GLsizei actualLength = 0;
        glGetActiveUniform(m_id, unif, nameData.size(), &actualLength, &arraySize, &type, &nameData[0]);
        std::string name((char*)&nameData[0], actualLength);

        if (name.find(".") == std::string::npos)
        {
            GLint location = glGetUniformLocation(m_id, name.c_str());

            // Insert the name and location into attribute hash table
            m_uniformAttributes[name] = location;
        }
    }
}

void ShaderProgram::queryActiveAttributes(const std::map<std::string, GLuint>& attribTable)
{
    GLint numActiveAttribs = 0;
    glGetProgramiv(m_id, GL_ACTIVE_ATTRIBUTES, &numActiveAttribs);

    std::vector<GLchar> nameData(256);

    for (int attrib = 0; attrib < numActiveAttribs; ++attrib)
    {
        GLint arraySize = 0;
        GLenum type = 0;
        GLsizei actualLength = 0;
        glGetActiveAttrib(m_id, attrib, nameData.size(), &actualLength, &arraySize, &type, &nameData[0]);
        std::string name((char*)&nameData[0], actualLength);

        auto attribute = attribTable.find(name);

        if (attribute != attribTable.end())
        {
            glBindAttribLocation(m_id, attribute->second, attribute->first.c_str());
        }
    }
}

}