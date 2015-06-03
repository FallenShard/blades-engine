#include <fstream>
#include <sstream>
#include "Renderer/ShaderManager.h"
#include "OglWrapper/VertexStreams.h"

namespace fsi
{

ShaderManager::ShaderManager()
    : m_relativePath("res/")
{
    m_attributeTable["position"] = VertexAttrib::Position;
    m_attributeTable["normal"] = VertexAttrib::Normal;
    m_attributeTable["texCoord0"] = VertexAttrib::TexCoord0;
    m_attributeTable["color"] = VertexAttrib::Color;

    loadShaderConfig(m_relativePath + "ShaderList.txt");
    loadProgramConfig(m_relativePath + "ProgramList.txt");
    
    // Don't need the shaders anymore
    m_shaders.clear();
}

ShaderManager::~ShaderManager()
{
    m_shaders.clear();
    m_programCache.clear();
}

void ShaderManager::loadShader(const std::string& fileName)
{
    Shader::Type type = getShaderType(fileName);

    if (type != Shader::None)
    {
        std::shared_ptr<Shader> shader = std::make_shared<Shader>(fileName, type);
        shader->checkForErrors();
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

void ShaderManager::loadProgramConfig(const std::string& fileName)
{
    std::ifstream inputFile;
    inputFile.open(fileName);

    if (inputFile.is_open())
    {
        std::vector<std::string> elements;

        // Obtain a line from the file for each program
        std::string line;
        while (std::getline(inputFile, line))
        {
            // This is where the splitting occurs
            std::stringstream strStream(line);
            std::string item;
            while (std::getline(strStream, item, ' '))
            {
                elements.emplace_back(item);
            }

            std::string programName = elements[0];
            std::vector<std::string> shaders(elements.begin() + 1, elements.end());

            buildProgram(programName, shaders);

            elements.clear();
        }
    }
}

std::shared_ptr<Shader> ShaderManager::getShader(const std::string& fileName)
{
    if (m_shaders[fileName] == nullptr)
        loadShader(fileName);

    return m_shaders[fileName];
}

void ShaderManager::buildProgram(const std::string& key, const std::vector<std::string>& shaderNames)
{
    std::unique_ptr<ShaderProgram> program = std::make_unique<ShaderProgram>();
    
//    ShaderProgram* program = new ShaderProgram();

    for (auto& shaderName : shaderNames)
    {
        program->attachShader(getShader(shaderName));
    }

    program->link();
    program->checkForErrors();

    // Bind vertex attribute to indices
    program->queryActiveAttributes(m_attributeTable);
    //program->bindVertexAttribute(VertexAttrib::Position,  "position");
    //program->bindVertexAttribute(VertexAttrib::Normal,    "normal");
    //program->bindVertexAttribute(VertexAttrib::TexCoord0, "texCoord");

    // Get all uniforms in the shader sources
    program->queryActiveUniforms();

    m_programCache[key] = std::move(program);
}

ShaderProgram* ShaderManager::getProgram(const std::string& key)
{
    return m_programCache[key].get();
}

Shader::Type ShaderManager::getShaderType(const std::string& fileName)
{
    std::string::size_type index;
    std::string extension;
    index = fileName.rfind(".");
    extension = fileName.substr(index + 1);

    if (extension == "vert")
    {
        return Shader::Vert;
    }
    else if (extension == "frag")
    {
        return Shader::Frag;
    }
    else if (extension == "tesc")
    {
        return Shader::TessControl;
    }
    else if (extension == "tese")
    {
        return Shader::TessEval;
    }
    else if (extension == "geom")
    {
        return Shader::Geom;
    }
    else
    {
        return Shader::None;
    }
}

}
