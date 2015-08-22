#pragma once

#include "OpenGL.h"

namespace fsi
{
    class Technique;

    class Sphere
    {
    public:
        Sphere(Technique* prog);

        ~Sphere();


        void init();

        void update(const float deltaTime);


        void render(const glm::mat4& projection, const glm::mat4& view);

    private:
        Technique* m_program;

        GLuint m_vao;
        GLuint m_vbo;
        GLuint m_ibo;

        GLuint m_ubo;
        GLuint m_sampler;

        glm::mat4 m_modelMatrix;
    };
}