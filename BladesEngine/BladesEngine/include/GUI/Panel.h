#pragma once

#include <memory>
#include <vector>

#include "OpenGL.h"

#include "GUI/Component.h"

namespace fsi
{
    class Technique;
    class GLRenderer;

    namespace gui
    {
        class Panel : public Component
        {
        public:
            Panel(GLRenderer* renderer, const glm::vec3& position = glm::vec3(0.f), const glm::vec2& size = glm::vec2(0.f));
            ~Panel();

            void setColor(const glm::vec4& color);
            void setBorderColor(const glm::vec4& color);

            virtual void render(const glm::mat4& P) override;

        private:
            std::vector<GLfloat> generateQuadVertices();
            std::vector<GLushort> generateQuadIndices();
            std::vector<GLushort> generateQuadBorderIndices();

            glm::vec4 m_color;
            glm::vec4 m_borderColor;

            GLuint m_borderVao;
            GLuint m_vao;
            GLuint m_vbo;
            GLuint m_ibo;
            GLuint m_borderIbo;

            std::unique_ptr<Technique> m_technique;
        };
    }
}