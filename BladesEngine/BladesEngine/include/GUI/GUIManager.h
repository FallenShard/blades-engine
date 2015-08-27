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
    class Component;
    class Text;
    class CheckBox;
    class GLRenderer;
    class Panel;
    class Slider;
    struct Event;

    namespace gui
    {
        class GUIManager
        {
        public:
            GUIManager(std::shared_ptr<Window>& window, GLRenderer* renderer, Scene* scene);

            ~GUIManager();

            bool handleEvents(const Event& event);

            void render();

            void setFrameTime(long long frameTime);

            void resize(int width, int height);

        private:
            std::shared_ptr<Font> m_font;

            glm::mat4 m_projection;

            std::shared_ptr<Panel> m_mainPanel;

            std::shared_ptr<Text> m_fpsCounterText;
            std::shared_ptr<Text> m_frameTimeText;

/*
                std::unique_ptr<Slider> m_triSlider;
                std::unique_ptr<Text> m_triSizeText;
                std::unique_ptr<Text> m_currTriSizeText;

                */
        };
    }
}