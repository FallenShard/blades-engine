#pragma once

#include <memory>
#include "OpenGL.h"

namespace fsi
{

class ShaderManager;
class Font;
class Window;
class Text;
class CheckBox;
class GLRenderer;
class Panel;
struct Event;

class UIRenderer
{
public:
    UIRenderer(std::shared_ptr<Window>& window, ShaderManager* program, GLRenderer* renderer);

    ~UIRenderer();

    bool handleEvents(const Event& event);

    void render();

    void setFrameTime(long long frameTime);

    void resize(int width, int height);

private:

    GLuint m_panelVao;
    GLuint m_panelVbo;

    ShaderProgram* m_program;
    
    ShaderProgram* m_textProgram;
    ShaderProgram* m_simpleProgram;

    glm::mat4 m_projection;

    std::shared_ptr<Font> m_font;

    std::unique_ptr<Panel> m_panel;

    std::unique_ptr<Text> m_aaText;
    std::unique_ptr<CheckBox> m_aaCheckBox;

    std::unique_ptr<Text> m_wireframeText;
    std::unique_ptr<CheckBox> m_wfCheckBox;

    std::unique_ptr<Text> m_vsyncText;
    std::unique_ptr<CheckBox> m_vsyncCheckBox;

    std::unique_ptr<Text> m_fpsCounter;
    std::unique_ptr<Text> m_frameTimeCounter;
};

}