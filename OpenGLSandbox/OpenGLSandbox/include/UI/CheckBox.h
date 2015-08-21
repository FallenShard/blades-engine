#pragma once

#include <memory>
#include <vector>
#include <functional>
#include "OpenGL.h"

namespace fsi
{
    struct Event;
    class Technique;
    class Text;

    class CheckBox
    {
    public:
        CheckBox(bool isChecked = false);
        ~CheckBox();

        bool handleEvents(const Event& event);

        void setPosition(glm::vec3& position);

        void render(Technique* program, const glm::mat4& parentMat);

        void addMouseArea(float xIncrease, float yIncrease = 0.f);
        glm::vec2 getSize() const;

        void setCallback(std::function<void(bool)> callback);

    private:
        void setState(bool isChecked);

        std::string m_textString;

        std::vector<GLfloat> m_vertices;
        std::vector<GLushort> m_indices;

        glm::mat4 m_modelMat;
        glm::vec2 m_position;
        glm::vec2 m_size;
        glm::vec2 m_mouseAreaSize;

        bool m_isChecked;

        GLuint m_vao;
        GLuint m_vbo;
        GLuint m_ibo;

        std::function<void(bool)> m_callback;
    };
}