#pragma once

#include <memory>
#include <vector>
#include <functional>

#include "OpenGL.h"

#include "GUI/Component.h"

namespace fsi
{
    class Font;
    class Technique;
    class GLRenderer;
    struct Event;

    namespace gui
    {
        class Text;

        class Slider : public Component
        {
        public:
            Slider(std::shared_ptr<Font>& font, GLRenderer* renderer, int values = 20, int defaultValue = 8);
            ~Slider();

            virtual void handleEvents(const Event& event) override;
            virtual void render(const glm::mat4& P) override;

            virtual void setPosition(const glm::vec3& position) override;
            virtual glm::vec4 getBounds() const override;

            int getValue() const;

            void setCallback(std::function<void(int)> callback);

        private:
            virtual void updateModelMatrix() override;
            void updateSliderPosition();
            std::vector<GLfloat> generateSliderVertices(float width, float height);
            std::vector<GLushort> generateSliderIndices();
            std::vector<GLfloat> generateBarVertices();

            int m_values;
            int m_currValue;
            std::function<void(int)> m_callback;

            std::shared_ptr<Text> m_valueText;

            glm::vec4 m_color;
            glm::mat4 m_sliderMat;

            glm::vec3 m_sliderPos;
            glm::vec2 m_sliderSize;

            bool m_isDragged;

            GLuint m_barVao;
            GLuint m_barVbo;

            GLuint m_sliderVao;
            GLuint m_sliderVbo;
            GLuint m_sliderIbo;

            std::unique_ptr<Technique> m_technique;
        };
    }
}