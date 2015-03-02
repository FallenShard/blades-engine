#pragma once

#include "OglWrapper/Shader.h"
#include "OglWrapper/ShaderProgram.h"

namespace fsi
{

class ShaderManager
{
public:
    ShaderManager();
    ~ShaderManager();

    // Retrieves a linked program from the cache
    ShaderProgram* getProgram(const std::string& key);

private:
    // Loads a config file which contains a list of all shaders to be loaded on start-up
    void loadShaderConfig(const std::string& fileName);

    // Loads a single shader and adds it to the map, using file name as key
    void loadShader(const std::string& fileName);

    // Loads a config file which contains a list of all shader programs to be loaded on start-up
    void loadProgramConfig(const std::string& fileName);

    // Builds a linked program with specified vector of shader file names
    void buildProgram(const std::string& key, const std::vector<std::string>& shaderNames);

    // Retrieves a shader with specified file name
    std::shared_ptr<Shader> getShader(const std::string& shaderFileName);

    // Determines shader type from file extension
    Shader::Type getShaderType(const std::string& fileName);

    std::string m_relativePath;

    std::map<std::string, GLuint> m_attributeTable;
    std::map<std::string, std::shared_ptr<Shader>> m_shaders;
    std::map<std::string, std::unique_ptr<ShaderProgram>> m_programCache;
};

}