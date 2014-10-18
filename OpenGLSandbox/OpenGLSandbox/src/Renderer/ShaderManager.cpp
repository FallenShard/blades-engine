#include <fstream>
#include "Renderer/ShaderManager.h"

ShaderManager::ShaderManager()
    : m_relativePath("res/")
{
    loadShaderConfig("res/ShaderList.txt");

    getProgram("default", "default.vert", "default.frag");
    getProgram("perspective", "motionVert.vert", "default.frag");
}

ShaderManager::~ShaderManager()
{
    for (auto& shader : m_shaders)
    {
        delete shader.second;
    }
    m_shaders.clear();

    for (auto& program : m_programCache)
    {
        delete program.second;
    }
    m_programCache.clear();
}

void ShaderManager::loadShader(const std::string& fileName)
{
    std::string::size_type index;
    std::string extension;
    index = fileName.rfind(".");
    extension = fileName.substr(index + 1);

    Shader* shader = nullptr;
    if (extension == "vert")
    {
        shader = new Shader(fileName, Shader::Vert);
    }
    else if (extension == "frag")
    {
        shader = new Shader(fileName, Shader::Frag);
    }
    else if (extension == "tesc")
    {
        shader = new Shader(fileName, Shader::TessControl);
    }
    else if (extension == "geom")
    {
        shader = new Shader(fileName, Shader::Geom);
    }

    if (shader != nullptr)
    {
        shader->checkCompileStatus();

        m_shaders[fileName] = shader;
    }
}

void ShaderManager::loadShaderConfig(const std::string& fileName)
{
    std::ifstream inputFile;
    inputFile.open(fileName);

    if (inputFile.is_open())
    {
        std::string line;
        while (std::getline(inputFile, line))
        {
            loadShader(line);
        }
        inputFile.close();
    }
}

Shader* ShaderManager::getShader(const std::string& fileName)
{
    if (m_shaders[fileName] == nullptr)
        loadShader(fileName);

    return m_shaders[fileName];
}

ShaderProgram* ShaderManager::getProgram(const std::string& key, const std::string& vShader, const std::string& fShader)
{
    Shader* vertShader = getShader(vShader);
    Shader* fragShader = getShader(fShader);

    assert(vertShader != nullptr);
    assert(fragShader != nullptr);

    ShaderProgram* program = new ShaderProgram();
    program->attachShader(*vertShader);
    program->attachShader(*fragShader);
    program->link();
    program->checkLinkStatus();

    m_programCache[key] = program;

    return program;
}

ShaderProgram* ShaderManager::getProgram(const std::string& key)
{
    return m_programCache[key];
}
