#pragma once

#include "Materials/Material.h"

namespace fsi
{

class HeightMapMaterial : public Material
{
public:
    HeightMapMaterial(ShaderProgram* program);
    virtual ~HeightMapMaterial();

    virtual void init();
    virtual void apply();
};

}