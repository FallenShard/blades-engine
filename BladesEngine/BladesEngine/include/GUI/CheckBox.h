#pragma once

#include <memory>
#include <vector>
#include <functional>

#include "OpenGL.h"

#include "GUI/Component.h"

namespace fsi
{
    struct Event;
    class GLRenderer;
    class Technique;
    class Font;

    namespace gui
    {
        class Text;

        class CheckBox : public Component
        {
        public:
            CheckBox(std::shared_ptr<Font>& font, GLRenderer* renderer, bool isChecked = false);
            ~CheckBox();

            virtual void handleEvents(const Event& event) override;
            virtual void render(const glm::mat4& P) override;

            void setCallback(std::function<void(bool)> callback);

            void setText(const std::string& text);
            virtual glm::vec4 getBounds() const override;

        private:
            void setState(bool isChecked);

            std::vector<GLfloat> generateVertices();
            std::vector<GLushort> generateIndices();

            bool m_isChecked;
            std::function<void(bool)> m_callback;

            std::shared_ptr<Text> m_text;
            glm::vec2 m_mouseAreaSize;

            GLRenderer* m_renderer;
            std::unique_ptr<Technique> m_technique;

            GLuint m_vao;
            GLuint m_vbo;
            GLuint m_ibo;
        };
    }
}