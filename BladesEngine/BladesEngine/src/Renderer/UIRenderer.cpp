#include "Renderer/VertexAssembly.h"
#include "Window/Window.h"
#include "Utils/Font.h"
#include "UI/Text.h"
#include "UI/CheckBox.h"
#include "UI/Panel.h"
#include "UI/Slider.h"
#include "Renderer/UIRenderer.h"
#include "Renderer/GLRenderer.h"
#include "Renderer/Scene.h"

#include "Models/Terrain.h"

#include <sstream>
#include <iomanip>
#include <iostream>

#include "stb/stb_image.h"

namespace fsi
{
    namespace
    {
        float avg = 0.f;

        int avgCount = 10;
        int avgCounter = 0;

        template <class T>
        inline std::string to_string(const T& t, int prec = 2)
        {
            std::stringstream ss;
            ss << std::setprecision(prec) << std::fixed << t;
            return ss.str();
        }

        PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;

        int updateIntervalMicroSec = 1000000;

        float panelX = 20.f;
        float panelY = 20.f;
        float panelWidth = 200.f;
        float panelHeight = 275.f;
    }

    UIRenderer::UIRenderer(std::shared_ptr<Window>& window, TechniqueCache* shaderManager, GLRenderer* renderer, Scene* scene)
        : m_font(std::make_shared<Font>(nullptr))
        //, m_textProgram(shaderManager->getProgram("text"))
        //, m_simpleProgram(shaderManager->getProgram("simple"))
        //, m_program(shaderManager->getProgram("gui"))
        , m_panel(std::make_unique<Panel>(glm::vec3(50.f, 50.f, 0.f), glm::vec2(panelWidth, panelHeight)))
        , m_aaText(std::make_unique<Text>(m_font))
        , m_aaCheckBox(std::make_unique<CheckBox>(true))
        , m_wireframeText(std::make_unique<Text>(m_font))
        , m_wfCheckBox(std::make_unique<CheckBox>())
        , m_vsyncText(std::make_unique<Text>(m_font))
        , m_vsyncCheckBox(std::make_unique<CheckBox>(true))
        , m_triSlider(std::make_unique<Slider>())
        , m_triSizeText(std::make_unique<Text>(m_font))
        , m_currTriSizeText(std::make_unique<Text>(m_font))
        , m_fpsCounter(std::make_unique<Text>(m_font))
        , m_frameTimeCounter(std::make_unique<Text>(m_font))
        , m_projection(glm::ortho(0.f, static_cast<float>(window->getSize().x),
        static_cast<float>(window->getSize().y), 0.f))
    {
        m_aaText->setText("Antialiasing");
        m_aaText->setColor(glm::vec4(0.f, 1.f, 1.f, 1.f));
        m_aaText->setPosition(glm::vec3(50.f, 40.f, 0.f));
        m_aaCheckBox->setPosition(glm::vec3(20.f, 20.f, 0.f));
        m_aaCheckBox->addMouseArea(m_aaText->getTextSize().x + 6.f);
        m_aaCheckBox->setCallback([renderer, scene](bool isChecked)
        {
            renderer->enableFXAA(isChecked);
            //scene->getTerrain()->useDetail(isChecked);
        });

        m_wireframeText->setText("Wireframe");
        m_wireframeText->setColor(glm::vec4(0.f, 1.f, 1.f, 1.f));
        m_wireframeText->setPosition(glm::vec3(50.f, 80.f, 0.f));
        m_wfCheckBox->setPosition(glm::vec3(20.f, 60.f, 0.f));
        m_wfCheckBox->addMouseArea(m_wireframeText->getTextSize().x + 6.f);
        m_wfCheckBox->setCallback([scene](bool isChecked)
        {
            scene->getTerrain()->setWireframe(isChecked);
        });

        m_vsyncText->setText("V-Sync");
        m_vsyncText->setColor(glm::vec4(0.f, 1.f, 1.f, 1.f));
        m_vsyncText->setPosition(glm::vec3(50.f, 120.f, 0.f));
        m_vsyncCheckBox->setPosition(glm::vec3(20.f, 100.f, 0.f));
        m_vsyncCheckBox->addMouseArea(m_vsyncText->getTextSize().x + 6.f);
        m_vsyncCheckBox->setCallback([](bool isChecked)
        {
            wglSwapIntervalEXT(isChecked);
        });

        m_triSizeText->setText("Triangle Size");
        m_triSizeText->setColor(glm::vec4(0.f, 1.f, 1.f, 1.f));
        m_triSizeText->setPosition(glm::vec3(20.f, 160.f, 0.f));
        m_currTriSizeText->setText("8");
        m_currTriSizeText->setColor(glm::vec4(0.f, 1.f, 1.f, 1.f));
        m_currTriSizeText->setPosition(glm::vec3(140.f, 185.f, 0.f));

        m_triSlider->setPosition(glm::vec3(20.f, 180.f, 0.f));
        m_triSlider->setCallback([scene, this](int value)
        {
            std::cout << value << std::endl;
            scene->getTerrain()->setTriSize(value);
            m_currTriSizeText->setText(to_string(value));
        });

        m_fpsCounter->setText("0.0 FPS");
        m_fpsCounter->setColor(glm::vec4(0.f, 1.f, 1.f, 1.f));
        m_fpsCounter->setPosition(glm::vec3(20.f, 220.f, 0.f));

        m_frameTimeCounter->setText("0.0 us");
        m_frameTimeCounter->setColor(glm::vec4(0.f, 1.f, 1.f, 1.f));
        m_frameTimeCounter->setPosition(glm::vec3(20.f, 250.f, 0.f));

        GLuint m_cbTexId = 0;
        GLuint m_cbTexUnif;
        GLuint m_cbTexUnit = 7;
        GLuint m_cbSampler = 0;

        int w, h, comp;
        unsigned char* img = stbi_load("res/checkbox.png", &w, &h, &comp, STBI_rgb_alpha);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_cbTexId);
        glTextureStorage2D(m_cbTexId, 2, GL_RGBA8, w, h);
        glTextureSubImage2D(m_cbTexId, 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, img);
        glGenerateTextureMipmap(m_cbTexId);

        glCreateSamplers(1, &m_cbSampler);
        glSamplerParameteri(m_cbSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glSamplerParameteri(m_cbSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glSamplerParameteri(m_cbSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glSamplerParameteri(m_cbSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        m_program->use();
        m_cbTexUnif = 0;// glGetUniformLocation(m_program->getProgramId(), "atlas");
        //m_program->setUniformSampler(m_cbTexUnif, m_cbTexUnit);

        glBindTextureUnit(m_cbTexUnit, m_cbTexId);
        glBindSampler(m_cbTexUnit, m_cbSampler);

        stbi_image_free(img);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
    }

    UIRenderer::~UIRenderer()
    {
    }

    bool UIRenderer::handleEvents(const Event& event)
    {
        if (event.type == Event::MouseButtonPressed ||
            event.type == Event::MouseButtonReleased)
        {
            if (event.mouseButton.x > panelX && event.mouseButton.x < panelX + panelWidth &&
                event.mouseButton.y > panelY && event.mouseButton.y < panelY + panelHeight)
            {
                m_aaCheckBox->handleEvents(event);
                m_wfCheckBox->handleEvents(event);
                m_vsyncCheckBox->handleEvents(event);
                m_triSlider->handleEvents(event);
                return true;
            }
        }
        else if (event.type == Event::MouseMoved)
        {
            if (event.mouseMove.x > panelX && event.mouseMove.x < panelX + panelWidth &&
                event.mouseMove.y > panelY && event.mouseMove.y < panelY + panelHeight)
            {
                m_triSlider->handleEvents(event);
                return true;
            }
        }

        return false;
    }

    void UIRenderer::render()
    {
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);

        m_simpleProgram->use();

        m_panel->render(m_simpleProgram, m_projection);

        glm::mat4 panelTrans = m_projection * glm::translate(m_panel->getPosition());

        m_program->use();

        m_aaCheckBox->render(m_program, panelTrans);
        m_wfCheckBox->render(m_program, panelTrans);
        m_vsyncCheckBox->render(m_program, panelTrans);

        m_textProgram->use();

        m_aaText->render(m_textProgram, panelTrans);
        m_wireframeText->render(m_textProgram, panelTrans);
        m_vsyncText->render(m_textProgram, panelTrans);
        m_triSizeText->render(m_textProgram, panelTrans);
        m_currTriSizeText->render(m_textProgram, panelTrans);
        m_frameTimeCounter->render(m_textProgram, panelTrans);
        m_fpsCounter->render(m_textProgram, panelTrans);

        m_triSlider->render(m_simpleProgram, m_textProgram, panelTrans);

        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
    }

    void UIRenderer::setFrameTime(long long frameTime)
    {
        avgCounter++;
        avg += frameTime;
        if (avg >= updateIntervalMicroSec)
        {
            float frameTime = avg / avgCounter;
            std::string frameTimeStr = to_string<float>(frameTime / 1000);
            std::string fpsStr = to_string<float>(1000000 / frameTime);

            m_fpsCounter->setText(fpsStr + " fps");
            m_frameTimeCounter->setText(frameTimeStr + " ms");
            avgCounter = 0;
            avg = 0;
        }
    }

    void UIRenderer::resize(int width, int height)
    {
        m_projection = glm::ortho(0.f, static_cast<float>(width), static_cast<float>(height), 0.f);
    }
}