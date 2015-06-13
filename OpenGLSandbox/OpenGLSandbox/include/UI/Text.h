#pragma once

#include <vector>
#include "OpenGL.h"

namespace fsi
{
    
class Text
{
public:
    Text(std::shared_ptr<Font>& textRenderer);
    ~Text();

    // void setFont(std::shared_ptr<Font>& font);

    void setText(std::string newText);
    void setPosition(glm::vec2 position);

    void render();


private:
    std::string m_textString;

    std::vector<GLfloat> m_vertices;
    std::vector<GLushort> m_indices;

    std::shared_ptr<Font> m_textRenderer;


    GLuint m_atlasTexId;

    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ibo;
};

}