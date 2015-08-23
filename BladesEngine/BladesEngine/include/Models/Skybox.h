#pragma once

#include "OpenGL.h"

#include "Renderer/DrawItem.h"

namespace fsi
{
    class Technique;
    class GLRenderer;

    class Skybox
    {
    public:
        Skybox(float size, GLRenderer* renderer);

        ~Skybox();

        void update(const glm::vec3& cameraPos);

    private:
        std::vector<GLfloat> generateVertices(float size);
        std::vector<GLushort> generateIndices();

        std::unique_ptr<Technique> m_technique;

        glm::mat4 m_modelMatrix;

        DrawItem m_drawItem;
    };
}