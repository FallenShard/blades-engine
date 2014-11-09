#pragma once

#include "OglWrapper/Shader.h"
#include "OglWrapper/ShaderProgram.h"

class ShaderManager
{
public:
    ShaderManager();
    ~ShaderManager();

    // Loads a single shader and adds it to the map, using file name as key
    void loadShader(const std::string& fileName);

    // Loads a config file which contains a list of all shaders to be loaded on start-up
    void loadShaderConfig(const std::string& fileName);

    // Retrieves a shader with specified file name
    Shader* getShader(const std::string& shaderFileName);

    // Retrieves a linked program with specified vertex and fragment shaders
    ShaderProgram* getProgram(const std::string& key, const std::string& vShader, const std::string& fShader);

    // Retrieves a linked program from the cache
    ShaderProgram* getProgram(const std::string& key);

private:
    std::string m_relativePath;

    std::map<std::string, Shader*> m_shaders;

    std::map<std::string, ShaderProgram*> m_programCache;
};