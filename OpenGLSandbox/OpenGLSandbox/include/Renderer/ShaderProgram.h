#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <memory>
#include <map>
#include "Renderer/Shader.h"

class ShaderProgram
{
public:
    ShaderProgram();
    ~ShaderProgram();

    void attachShader(Shader& shader);
    void attachShader(Shader::Type type, std::string fileName);
    void detachShader(Shader& shader);

    void compile();
    void link();
    void use();
    static void release();

    bool checkLinkStatus();

    GLint getUniformAttribute(std::string name);

    void setUniformAttribute(std::string name, GLfloat value);
    void setUniformAttribute(std::string name, GLfloat x, GLfloat y);
    void setUniformAttribute(std::string name, GLfloat x, GLfloat y, GLfloat z);
    void setUniformAttribute(std::string name, GLsizei count, GLboolean transpose, const GLfloat* values);

    GLuint getProgramId() const;

private:
    GLuint m_id;

    typedef std::shared_ptr<Shader> ShaderPtr;

    std::map<std::string, ShaderPtr> m_shaders;
    std::map<std::string, GLint> m_uniformAttributes;
};


#endif // SHADER_PROGRAM_H