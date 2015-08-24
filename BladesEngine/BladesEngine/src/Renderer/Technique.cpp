#include <vector>

#include "Utils/Logger.h"
#include "Renderer/Technique.h"

#include <iostream>

namespace
{
    GLuint prevBoundProgram = 0;
}

namespace fsi
{
    Technique::Technique(GLuint id)
        : m_id(id)
    {
        queryActiveUniforms();
        queryActiveBlocks();
    }

    Technique::~Technique()
    {
    }

    GLuint Technique::getRawHandle() const
    {
        return m_id;
    }

    void Technique::use()
    {
        if (prevBoundProgram != m_id)
        {
            prevBoundProgram = m_id;
            glUseProgram(m_id);
        }
    }

    void Technique::release()
    {
        if (prevBoundProgram != 0)
        {
            prevBoundProgram = 0;
            glUseProgram(0);
        }
    }

    GLint Technique::getUniformAttribute(const std::string& name)
    {
        return m_uniformAttributes[name];
    }

    GLint Technique::getUniformBlock(const std::string& name)
    {
        return m_uniformBlocks[name];
    }

    void Technique::setUniformAttribute(const std::string& name, GLfloat value)
    {
        glProgramUniform1f(m_id, m_uniformAttributes[name], value);
    }

    void Technique::setUniformAttribute(const std::string& name, GLint value)
    {
        glProgramUniform1i(m_id, m_uniformAttributes[name], value);
    }

    void Technique::setUniformAttribute(const std::string& name, GLuint value)
    {
        glProgramUniform1i(m_id, m_uniformAttributes[name], value);
    }

    void Technique::setUniformAttribute(const std::string& name, GLfloat x, GLfloat y)
    {
        glProgramUniform2f(m_id, m_uniformAttributes[name], x, y);
    }

    void Technique::setUniformAttribute(const std::string& name, GLfloat x, GLfloat y, GLfloat z)
    {
        glProgramUniform3f(m_id, m_uniformAttributes[name], x, y, z);
    }

    void Technique::setUniformAttribute(const std::string& name, GLsizei count, GLboolean transpose, const GLfloat* values)
    {
        glProgramUniformMatrix4fv(m_id, m_uniformAttributes[name], count, transpose, values);
    }

    void Technique::setUniformAttribute(const std::string& name, const glm::mat4& mat)
    {
        glProgramUniformMatrix4fv(m_id, m_uniformAttributes[name], 1, GL_FALSE, glm::value_ptr(mat));
    }

    void Technique::setUniformAttribute(const std::string& name, const glm::mat3& mat)
    {
        glProgramUniformMatrix3fv(m_id, m_uniformAttributes[name], 1, GL_FALSE, glm::value_ptr(mat));
    }

    void Technique::setUniformAttribute(const std::string& name, const glm::vec2& vec)
    {
        glProgramUniform2fv(m_id, m_uniformAttributes[name], 1, glm::value_ptr(vec));
    }

    void Technique::setUniformAttribute(const std::string& name, const glm::vec3& vec)
    {
        glProgramUniform3fv(m_id, m_uniformAttributes[name], 1, glm::value_ptr(vec));
    }

    void Technique::setUniformAttribute(const std::string& name, const glm::vec4& vec)
    {
        glProgramUniform4fv(m_id, m_uniformAttributes[name], 1, glm::value_ptr(vec));
    }

    UniformBufferDescriptor Technique::createUniformBufferDescriptor(const std::string& blockName, GLuint binding, GLuint buffer)
    {
        UniformBufferDescriptor ubd;
        ubd.binding = binding;
        ubd.buffer = buffer;

        glUniformBlockBinding(m_id, m_uniformBlocks[blockName], ubd.binding);
        glBindBufferBase(GL_UNIFORM_BUFFER, ubd.binding, ubd.buffer);

        return ubd;
    }

    void Technique::queryActiveUniforms()
    {
        GLint numUniforms = 0;
        glGetProgramInterfaceiv(m_id, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms);
        const GLenum properties[4] = { GL_BLOCK_INDEX, GL_TYPE, GL_NAME_LENGTH, GL_LOCATION };

        for (int unif = 0; unif < numUniforms; ++unif)
        {
            GLint values[4];
            glGetProgramResourceiv(m_id, GL_UNIFORM, unif, 4, properties, 4, nullptr, values);

            if (values[0] != -1)
                continue;

            std::vector<GLchar> nameVec(values[2]);
            glGetProgramResourceName(m_id, GL_UNIFORM, unif, values[2], nullptr, &nameVec[0]);
            std::string name(nameVec.begin(), --nameVec.end()); //-- in order to remove '\0'

            m_uniformAttributes[name] = values[3];
        }
    }

    void Technique::queryActiveBlocks()
    {
        GLint numBlocks = 0;
        glGetProgramInterfaceiv(m_id, GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES, &numBlocks);
        const GLenum properties[1] = { GL_NAME_LENGTH };

        for (int block = 0; block < numBlocks; ++block)
        {
            GLint nameLength;
            glGetProgramResourceiv(m_id, GL_UNIFORM_BLOCK, block, 1, properties, 1, nullptr, &nameLength);

            std::vector<GLchar> nameVec(nameLength);
            glGetProgramResourceName(m_id, GL_UNIFORM_BLOCK, block, nameLength, nullptr, &nameVec[0]);
            std::string name(nameVec.begin(), --nameVec.end()); //-- in order to remove '\0'

            m_uniformBlocks[name] = block;
        }
    }
}