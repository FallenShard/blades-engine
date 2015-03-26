#pragma once

#include <vector>

#include "OglWrapper/ShaderProgram.h"
#include "OglWrapper/VertexArray.h"

namespace fsi
{

struct RenderPackage
{
    // Pipeline state
    //ShaderProgram* program;

    // Input asssembly state
    GLuint vertexArrayId;
    GLenum primitiveType;

    // Resource bindings
    //std::map<std::string, GLuint> uniforms;
    //std::vector<std::pair<int, std::shared_ptr<UniformBuffer>>> uniformBuffers;
    //std::vector<std::pair<int, int>> samplers;
    //std::vector<std::pair<int, int>> textures;

    // Draw call
    GLuint vertexCount;
    GLuint vertexStart;
    GLuint indexCount;
    GLuint indexStart;
};

}
