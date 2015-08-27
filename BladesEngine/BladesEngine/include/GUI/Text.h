#pragma once

#include <vector>
#include <memory>

#include "OpenGL.h"

#include "GUI/Component.h"

namespace fsi
{
    class Font;
    class Technique;
    class GLRenderer;

    namespace gui
    {
        class Text : public Component
        {
        public:
            Text(std::shared_ptr<Font>& font, GLRenderer* renderer);
            ~Text();

            void setText(std::string newText);
            void setColor(const glm::vec4& color);
            glm::vec2 getTextSize() const;
            virtual glm::vec4 getBounds() const override;

            virtual void render(const glm::mat4& P) override;

        private:
            std::string m_textString;
            glm::vec2 m_textSize;

            glm::vec4 m_color;

            std::shared_ptr<Font> m_font;
            GLRenderer* m_renderer;
            std::unique_ptr<Technique> m_technique;

            std::vector<GLfloat> m_vertices;
            std::vector<GLushort> m_indices;

            GLuint m_vbo;
            GLuint m_ibo;
            GLuint m_vao;
        };
    }
}