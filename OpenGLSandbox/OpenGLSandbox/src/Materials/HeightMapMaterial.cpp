#include "OpenGL.h"

#include "OglWrapper/ShaderProgram.h"

#include "Materials/HeightMapMaterial.h"

namespace fsi
{

HeightMapMaterial::HeightMapMaterial(ShaderProgram* program)
{
    m_program = program;
}

HeightMapMaterial::~HeightMapMaterial()
{

}

void HeightMapMaterial::init()
{
    glPatchParameteri(GL_PATCH_VERTICES, 4);
}

void HeightMapMaterial::apply()
{
    m_program->use();
}

}