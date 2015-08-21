#pragma once

#include <vector>
#include "OpenGL.h"

namespace fsi
{
    class Font;
    class Technique;
    
    class Text
    {
    public:
        Text(std::shared_ptr<Font>& font);
        ~Text();

        void setText(std::string newText);
        void setPosition(glm::vec3&& position);
        void setColor(glm::vec4&& color);

        glm::vec2 getTextSize() const;

        void render(Technique* prog, const glm::mat4& parent);


    private:
        std::string m_textString;

        std::vector<GLfloat> m_vertices;
        std::vector<GLushort> m_indices;

        std::shared_ptr<Font> m_font;

        glm::vec4 m_color;
        glm::mat4 m_modelMat;

        glm::vec2 m_size;

        GLuint m_atlasTexId;

        GLuint m_vao;
        GLuint m_vbo;
        GLuint m_ibo;
    };
}