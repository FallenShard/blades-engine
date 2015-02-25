#pragma once

#include "OpenGL.h"

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