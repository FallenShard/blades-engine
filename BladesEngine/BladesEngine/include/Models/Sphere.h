#pragma once

#include "OpenGL.h"

#include "Renderer/DrawItem.h"

namespace fsi
{
    class GLRenderer;
    class Technique;

    class Sphere
    {
    public:
        Sphere(float radius, int rings, int slices, GLRenderer* renderer);
        ~Sphere();

        void update(const float deltaTime);

    private:
        struct Material
        {
            glm::vec4 ambient;
            glm::vec4 diffuse;
            glm::vec4 specular;
            float shininess;
        };

        void generateGeometry(std::vector<GLfloat>& vertices, std::vector<GLushort>& indices, float radius, int rings, int slices);

        std::unique_ptr<Technique> m_technique;

        glm::mat4 m_modelMatrix;

        DrawItem m_drawItem;
    };
}