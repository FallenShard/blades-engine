#include <iostream>
#include <sstream>
#include <iomanip>

#include "Renderer/VertexAssembly.h"
#include "Window/Window.h"
#include "Utils/Font.h"
#include "Utils/Math.h"
#include "Utils/StringConversion.h"
#include "GUI/Text.h"
#include "GUI/CheckBox.h"
#include "GUI/Panel.h"
#include "GUI/Slider.h"
#include "GUI/GUIManager.h"
#include "Renderer/GLRenderer.h"
#include "Renderer/Scene.h"

#include "Models/Terrain.h"

namespace fsi
{
    namespace
    {
        float avg = 0.f;
        int avgCounter = 0;
        int updateIntervalMicroSec = 1000000;

        

        PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
    }

    namespace gui
    {
        GUIManager::GUIManager(std::shared_ptr<Window>& window, GLRenderer* renderer, Scene* scene)
            : m_font(std::make_shared<Font>(renderer, 18))
            , m_projection(glm::ortho(0.f, static_cast<float>(window->getSize().x), static_cast<float>(window->getSize().y), 0.f))
            , m_mainPanel(nullptr)
            , m_fpsCounterText(nullptr)
            , m_frameTimeText(nullptr)
        {
            m_mainPanel = std::make_shared<Panel>(renderer);
            m_mainPanel->setPosition({ 20.f, 50.f, 0.f });
            m_mainPanel->setSize({ 200.f, 200.f });
            m_mainPanel->setColor({ 0.f, 0.f, 0.f, 0.3f });

            m_fpsCounterText = std::make_shared<Text>(m_font, renderer);
            m_fpsCounterText->setText("");
            m_fpsCounterText->setPosition({ 20.f, 30.f, 0.f });
            m_fpsCounterText->setColor({ 0.f, 1.f, 1.f, 1.f });

            m_frameTimeText = std::make_shared<Text>(m_font, renderer);
            m_frameTimeText->setText("");
            m_frameTimeText->setPosition({ 20.f, 60.f, 0.f });
            m_frameTimeText->setColor({ 0.f, 1.f, 1.f, 1.f });

            auto antiAliasingCheckBox = std::make_shared<CheckBox>(m_font, renderer, true);
            antiAliasingCheckBox->setText("Antialiasing");
            antiAliasingCheckBox->setPosition({ 20.f, 70.f, 0.f });
            antiAliasingCheckBox->setCallback([](bool isChecked)
            {
                std::cout << "Anti-aliasing CheckBox checked status: " << isChecked << std::endl;
            });

            auto wireframeCheckBox = std::make_shared<CheckBox>(m_font, renderer, false);
            wireframeCheckBox->setText("Wireframe");
            wireframeCheckBox->setPosition({ 20.f, 100.f, 0.f });
            wireframeCheckBox->setCallback([renderer](bool isChecked)
            {
                renderer->setWireframeMode(isChecked);
            });

            auto vSyncCheckBox = std::make_shared<CheckBox>(m_font, renderer, true);
            vSyncCheckBox->setText("V-Sync");
            vSyncCheckBox->setPosition({ 20.f, 130.f, 0.f });
            vSyncCheckBox->setCallback([](bool isChecked)
            {
                wglSwapIntervalEXT(isChecked);
            });

            auto triangleSizeSlider = std::make_shared<Slider>(m_font, renderer);
            triangleSizeSlider->setPosition({ 20.f, 180.f, 0.f });
            triangleSizeSlider->setCallback([](int value)
            {
                std::cout << "Slider value: " << value << std::endl;
            });

            m_mainPanel->addComponent(m_fpsCounterText);
            m_mainPanel->addComponent(m_frameTimeText);
            m_mainPanel->addComponent(antiAliasingCheckBox);
            m_mainPanel->addComponent(wireframeCheckBox);
            m_mainPanel->addComponent(vSyncCheckBox);
            m_mainPanel->addComponent(triangleSizeSlider);

            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
        }

        GUIManager::~GUIManager()
        {
        }

        bool GUIManager::handleEvents(const Event& event)
        {
            int mouseX;
            int mouseY;

            if (event.type == Event::MouseButtonPressed || event.type == Event::MouseButtonReleased)
            {
                mouseX = event.mouseButton.x;
                mouseY = event.mouseButton.y;
            }
            else if (event.type == Event::MouseMoved)
            {
                mouseX = event.mouseMove.x;
                mouseY = event.mouseMove.y;
            }
            else
                return false;

            auto bounds = m_mainPanel->getBounds();
            if (isPointInRect(mouseX, mouseY, bounds))
            {
                m_mainPanel->handleEvents(event);
                return true;
            }

            return false;
        }

        void GUIManager::render()
        {
            glDisable(GL_DEPTH_TEST);
            glEnable(GL_BLEND);

            m_mainPanel->render(m_projection);
            
            glDisable(GL_BLEND);
            glEnable(GL_DEPTH_TEST);
        }

        void GUIManager::setFrameTime(long long frameTime)
        {
            avgCounter++;
            avg += frameTime;
            if (avg >= updateIntervalMicroSec)
            {
                float frameTime = avg / avgCounter;
                std::string frameTimeStr = to_string<float>(frameTime / 1000);
                std::string fpsStr = to_string<float>(1000000 / frameTime);

                m_fpsCounterText->setText(fpsStr + " FPS");
                m_frameTimeText->setText(frameTimeStr + " ms");
                avgCounter = 0;
                avg = 0;
            }
        }

        void GUIManager::resize(int width, int height)
        {
            m_projection = glm::ortho(0.f, static_cast<float>(width), static_cast<float>(height), 0.f);
        }
    }
}