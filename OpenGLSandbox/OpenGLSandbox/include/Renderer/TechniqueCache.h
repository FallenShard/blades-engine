#pragma once

#include <map>
#include <set>

#include "OpenGL.h"

#include "Renderer/Technique.h"

namespace fsi
{
    class TechniqueCache
    {
    public:
        enum class ShaderType
        {
            None,
            Vertex,
            TessControl,
            TessEvaluation,
            Geometry,
            Fragment,
            Compute
        };

        TechniqueCache();
        ~TechniqueCache();

        GLuint getProgram(const std::string& name);

    private:
        typedef std::map<std::string, GLuint> ResourceMap;

        void prepareShader(ResourceMap& shaderMap, const std::string& fileName, const ShaderType type);
        void assembleTechnique(const std::string& techniqueName, const std::set<std::string>& shaderSet, ResourceMap& shaderMap);
        
        GLenum determineRawType(const ShaderType type);
        ShaderType determineTypeFromXml(const std::string& value);

        std::string fileToString(const std::string& fileName);

        static const std::string relativePath;
        ResourceMap m_techniqueMap;
    };
}