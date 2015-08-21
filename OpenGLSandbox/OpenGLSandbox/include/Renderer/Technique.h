#pragma once

#include <memory>
#include <map>
#include "OpenGL.h"

namespace fsi
{
    struct UniformBufferDescriptor
    {
        GLuint binding;
        GLuint buffer;
    };

    class Technique
    {
    public:
        Technique(GLuint id);
        ~Technique();

        void use();
        static void release();

        GLint getUniformAttribute(const std::string& name);
        GLint getUniformBlock(const std::string& name);

        void setUniformAttribute(const std::string& name, GLfloat value);
        void setUniformAttribute(const std::string& name, GLint value);
        void setUniformAttribute(const std::string& name, GLfloat x, GLfloat y);
        void setUniformAttribute(const std::string& name, GLfloat x, GLfloat y, GLfloat z);
        void setUniformAttribute(const std::string& name, GLsizei count, GLboolean transpose, const GLfloat* values);
        void setUniformAttribute(const std::string& name, const glm::mat3& mat);
        void setUniformAttribute(const std::string& name, const glm::mat4& mat);
        void setUniformAttribute(const std::string& name, const glm::vec2& vec);
        void setUniformAttribute(const std::string& name, const glm::vec3& vec);
        void setUniformAttribute(const std::string& name, const glm::vec4& vec);

        UniformBufferDescriptor createUniformBufferDescriptor(const std::string& blockName, GLuint binding, GLuint buffer);

    private:
        void queryActiveUniforms();
        void queryActiveBlocks();

        GLuint m_id;

        std::map<std::string, GLint> m_uniformAttributes;
        std::map<std::string, GLint> m_uniformBlocks;
    };
}