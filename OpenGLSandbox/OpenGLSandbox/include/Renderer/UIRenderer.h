#pragma once

#include <memory>
#include "OpenGL.h"

namespace fsi
{
    class TechniqueCache;
    class Technique;
    class Scene;
    class Font;
    class Window;
    class Text;
    class CheckBox;
    class GLRenderer;
    class Panel;
    class Slider;
    struct Event;

    class UIRenderer
    {
    public:
        UIRenderer(std::shared_ptr<Window>& window, TechniqueCache* shaderManager, GLRenderer* renderer, Scene* Scene);

        ~UIRenderer();

        bool handleEvents(const Event& event);

        void render();

        void setFrameTime(long long frameTime);

        void resize(int width, int height);

    private:

        GLuint m_panelVao;
        GLuint m_panelVbo;

        Technique* m_program;
        Technique* m_textProgram;
        Technique* m_simpleProgram;

        glm::mat4 m_projection;

        std::shared_ptr<Font> m_font;

        std::unique_ptr<Panel> m_panel;

        std::unique_ptr<Text> m_aaText;
        std::unique_ptr<CheckBox> m_aaCheckBox;

        std::unique_ptr<Text> m_wireframeText;
        std::unique_ptr<CheckBox> m_wfCheckBox;

        std::unique_ptr<Text> m_vsyncText;
        std::unique_ptr<CheckBox> m_vsyncCheckBox;

        std::unique_ptr<Slider> m_triSlider;
        std::unique_ptr<Text> m_triSizeText;
        std::unique_ptr<Text> m_currTriSizeText;

        std::unique_ptr<Text> m_fpsCounter;
        std::unique_ptr<Text> m_frameTimeCounter;
    };
}