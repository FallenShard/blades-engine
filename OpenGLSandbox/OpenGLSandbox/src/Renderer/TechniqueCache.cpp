#include <iostream>
#include <fstream>
#include <vector>

#include "rapidXML/rapidxml.hpp"

#include "Renderer/TechniqueCache.h"

namespace fsi
{
    const std::string TechniqueCache::relativePath("res/");

    TechniqueCache::TechniqueCache()
    {
        std::string xmlConfig = fileToString(relativePath + "techniques.xml");

        using namespace rapidxml;
        xml_document<> doc;
        doc.parse<0>(&xmlConfig[0]);
        xml_node<>* root = doc.first_node();

        std::set<std::pair<std::string, ShaderType>> shaderSourceSet;
        std::map<std::string, std::set<std::string>> techniqueSrcMap;
        for (auto node = root->first_node("technique"); node; node = node->next_sibling())
        {
            auto techAttrib = node->first_attribute("name");
            if (techAttrib)
            {
                std::string techName = techAttrib->value();
                techniqueSrcMap[techName] = std::set<std::string>();

                for (auto shaderNode = node->first_node("shader"); shaderNode; shaderNode = shaderNode->next_sibling())
                {
                    auto srcAttrib = shaderNode->first_attribute("source");
                    auto typeAttrib = shaderNode->first_attribute("type");
                    if (srcAttrib && typeAttrib)
                    {
                        shaderSourceSet.emplace(srcAttrib->value(), determineTypeFromXml(typeAttrib->value()));
                        techniqueSrcMap[techName].emplace(srcAttrib->value());
                    }
                }
            }
        }

        std::map<std::string, GLuint> shaderMap;
        for (auto& shaderFile : shaderSourceSet)
            prepareShader(shaderMap, shaderFile.first, shaderFile.second);

        for (auto& techniqueSrc : techniqueSrcMap)
            assembleTechnique(techniqueSrc.first, techniqueSrc.second, shaderMap);

        for (auto& shader : shaderMap)
            glDeleteShader(shader.second);
    }

    TechniqueCache::~TechniqueCache()
    {
        for (auto& technique : m_techniqueMap)
            glDeleteProgram(technique.second);
    }

    GLuint TechniqueCache::getProgram(const std::string& name)
    {
        auto found = m_techniqueMap.find(name);

        if (found != m_techniqueMap.end())
            return found->second;
        else
            return 0;
    }

    void TechniqueCache::prepareShader(TechniqueCache::ResourceMap& shaderMap, const std::string& fileName, const ShaderType type)
    {
        GLenum glType = determineRawType(type);
        if (glType == GL_INVALID_ENUM)
        {
            std::cout << "Failed to determine shader type for " << fileName << std::endl;
            return;
        }

        GLuint shader = glCreateShader(glType);

        std::string srcString = fileToString(relativePath + "glsl/" + fileName);
        const GLchar* src = srcString.c_str();
        glShaderSource(shader, 1, &src, nullptr);
        
        glCompileShader(shader);
        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            std::cout << "Error compiling shader " << fileName << "!" << std::endl;
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> errorLog(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
            
            std::string error(errorLog.begin(), errorLog.end());
            std::cout << error << std::endl;

            glDeleteShader(shader);
            return;
        }

        shaderMap[fileName] = shader;
    }

    void TechniqueCache::assembleTechnique(const std::string& techniqueName, const std::set<std::string>& shaderSet, TechniqueCache::ResourceMap& shaderMap)
    {
        GLuint program = glCreateProgram();

        for (auto& shaderName : shaderSet)
            glAttachShader(program, shaderMap[shaderName]);
        
        glLinkProgram(program);
        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
        if (isLinked == GL_FALSE)
        {
            std::cout << "Error linking program " << techniqueName << "!" << std::endl;
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> errorLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &errorLog[0]);

            std::string error(errorLog.begin(), errorLog.end());
            std::cout << error << std::endl;

            glDeleteProgram(program);
            return;
        }

        for (auto& shaderName : shaderSet)
            glDetachShader(program, shaderMap[shaderName]);

        m_techniqueMap[techniqueName] = program;
    }

    GLenum TechniqueCache::determineRawType(const ShaderType type)
    {
        switch (type)
        {
        case ShaderType::Vertex:         return GL_VERTEX_SHADER;
        case ShaderType::TessControl:    return GL_TESS_CONTROL_SHADER;
        case ShaderType::TessEvaluation: return GL_TESS_EVALUATION_SHADER;
        case ShaderType::Geometry:       return GL_GEOMETRY_SHADER;
        case ShaderType::Fragment:       return GL_FRAGMENT_SHADER;
        case ShaderType::Compute:        return GL_COMPUTE_SHADER;
        default:                         return GL_INVALID_ENUM;
        }
    }

    TechniqueCache::ShaderType TechniqueCache::determineTypeFromXml(const std::string& value)
    {
        if (value == "Vertex")         return ShaderType::Vertex;
        if (value == "TessControl")    return ShaderType::TessControl;
        if (value == "TessEvaluation") return ShaderType::TessEvaluation;
        if (value == "Geometry")       return ShaderType::Geometry;
        if (value == "Fragment")       return ShaderType::Fragment;
        if (value == "Compute")        return ShaderType::Compute;

        return ShaderType::None;
    }

    std::string TechniqueCache::fileToString(const std::string& fileName)
    {
        std::ifstream inputFile(fileName);
        std::string source;

        if (inputFile.is_open())
        {
            std::string line;
            while (std::getline(inputFile, line))
            {
                source += line + '\n';
            }
            inputFile.close();
        }

        return source;
    }
}