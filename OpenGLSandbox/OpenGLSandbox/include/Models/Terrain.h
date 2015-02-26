#pragma once

#include "OpenGL.h"

namespace fsi
{

class Terrain
{
public:
    Terrain();
    ~Terrain();


    void init();

    void update(const float deltaTime);


    void render(const glm::mat4& projection, const glm::mat4& view);

private:
    
};

}