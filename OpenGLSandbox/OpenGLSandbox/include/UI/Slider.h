#pragma once

#include <memory>
#include <vector>
#include <functional>
#include "OpenGL.h"

namespace fsi
{
    class Font;
    class Technique;
    struct Event;

    class Slider
    {
    public:
        Slider();
        ~Slider();

        void handleEvents(const Event& event);
        void render(Technique* prog, Technique* textProg, const glm::mat4& parent);
    
        void setPosition(glm::vec3& position);

        void setCallback(std::function<void(int)> callback);

    private:

        glm::vec4 m_color;
        glm::mat4 m_modelMat;
        glm::mat4 m_sliderMat;

        glm::vec2 m_position;
        glm::vec2 m_size;

        glm::vec2 m_sliderPos;
        glm::vec2 m_sliderSize;


        GLuint m_barVao;
        GLuint m_barVbo;

        GLuint m_sliderVao;
        GLuint m_sliderVbo;
        GLuint m_sliderIbo;

        bool m_isDragged;

        int m_value;
        std::function<void(int)> m_callback;
    };
}