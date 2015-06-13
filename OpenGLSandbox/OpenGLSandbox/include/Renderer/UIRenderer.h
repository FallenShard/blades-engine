#pragma once

#include <memory>
#include "OpenGL.h"

namespace fsi
{

class ShaderManager;
class Font;
class Window;
class Text;
struct Event;

class UIRenderer
{
public:
    UIRenderer(std::shared_ptr<Window>& window, ShaderManager* program);

    ~UIRenderer();

    bool handleEvents(const Event& event);

    void render();

    void resize(int width, int height);

private:

    GLuint m_panelVao;
    GLuint m_panelVbo;

    ShaderManager* m_program;
    
    ShaderProgram* m_textProgram;
    ShaderProgram* m_simpleProgram;

    glm::mat4 m_projection;

    std::shared_ptr<Font> m_textRenderer;

    std::unique_ptr<Text> m_aaText;
    std::unique_ptr<Text> m_wireframeText;
};

}