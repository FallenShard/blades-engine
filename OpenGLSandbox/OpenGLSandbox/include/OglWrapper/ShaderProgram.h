#pragma once

#include <memory>
#include <map>
#include "OglWrapper/Shader.h"

namespace fsi
{

class ShaderProgram
{
public:
    ShaderProgram();
    ~ShaderProgram();

    void attachShader(Shader& shader);
    void attachShader(std::shared_ptr<Shader> shader);
    void attachShader(Shader::Type type, std::string fileName);
    void detachShader(Shader& shader);

    void compile();
    void link();
    void use();
    static void release();

    bool checkForErrors();

    void queryActiveUniforms();
    void queryActiveAttributes(const std::map<std::string, GLuint>& attribTable);

    GLint getUniformAttribute(std::string name);
    GLint getUniformBuffer(std::string name);

    void bindVertexAttribute(GLuint index, std::string attribName);

    void setUniformAttribute(std::string name, GLfloat value);
    void setUniformAttribute(std::string name, GLint value);
    void setUniformAttribute(std::string name, GLfloat x, GLfloat y);
    void setUniformAttribute(std::string name, GLfloat x, GLfloat y, GLfloat z);
    void setUniformAttribute(std::string name, GLsizei count, GLboolean transpose, const GLfloat* values);
    void setUniformAttribute(std::string name, const glm::mat3& mat);
    void setUniformAttribute(std::string name, const glm::mat4& mat);
    void setUniformAttribute(std::string name, const glm::vec3& vec);
    void setUniformAttribute(std::string name, const glm::vec4& vec);
    
    void setUniformSampler(std::string name, GLint textureUnit);
    void setUniformSampler(GLint location, GLint textureUnit);

    void setUniformBlockBinding(GLuint blockIndex, GLuint bindingPoint);
    void setUniformBlockBinding(std::string blockName, GLuint bindingPoint);

    GLuint getProgramId() const;

private:
    GLuint m_id;

    std::map<std::string, std::shared_ptr<Shader>> m_shaders;
    std::map<std::string, GLint> m_uniformAttributes;
};

}