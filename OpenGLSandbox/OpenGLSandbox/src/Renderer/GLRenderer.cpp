#include "Renderer/GLRenderer.h"
#include "Window/Event.h"

#include "Scenes/TriangleScene.h"
#include "Scenes/PrismScene.h"
#include "Scenes/OverlapScene.h"
#include "Scenes/TranslationScene.h"
#include "Scenes/GraphScene.h"
#include "Scenes/RobotArmScene.h"

#include "Renderer/FrameBuffer.h"
#include "Renderer/Texture.h"

namespace
{
    bool useFXAA = true;
}

GLRenderer::GLRenderer(Window* window)
    : m_aspectRatio(-1.f)
    , m_timePassed(0.f)
    , m_window(window)
    , m_shaderManager(nullptr)
    , m_aaPass(nullptr)
{
    init();
}

GLRenderer::GLRenderer(int width, int height)
    : m_aspectRatio(static_cast<float>(width) / height)
    , m_timePassed(0.f)
{
    init();
}

GLRenderer::~GLRenderer()
{
    for (auto& scene : m_scenes)
        delete scene;
    m_scenes.clear();

    delete m_aaPass;
}

void GLRenderer::init()
{
    if (m_window)
    {
        glm::ivec2 windowSize = m_window->getSize();
        m_aspectRatio = static_cast<float>(windowSize.x) / windowSize.y;
    }

    m_shaderManager = new ShaderManager();

    Scene* scene = new RobotArmScene(m_window, m_shaderManager);
    m_scenes.push_back(scene);

    for (auto& scene : m_scenes)
        scene->prepare();
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
    
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);

    glClearColor(0.1f, 0.1f, 0.1f, 1.f);
    glClearDepth(1.0f);

    glm::ivec2 windowSize = m_window->getSize();

    m_aaPass = new RenderPass();
    m_aaPass->attachProgram(m_shaderManager->getProgram("FXAAPass"));
    m_aaPass->init(windowSize.x, windowSize.y);
}

void GLRenderer::draw()
{
    if (useFXAA)
    {
        m_aaPass->activate();

        for (auto& scene : m_scenes)
            scene->render();

        FrameBuffer::bindScreen();
        FrameBuffer::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_aaPass->render();
    }
    else
    {
        FrameBuffer::bindScreen();
        FrameBuffer::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (auto& scene : m_scenes)
            scene->render();
    }
}

void GLRenderer::handleEvents(const Event& event)
{
    if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space)
        useFXAA = !useFXAA;

    for (auto& scene : m_scenes)
        scene->handleEvents(event);
}

void GLRenderer::update(float timeDelta)
{
    m_timePassed += timeDelta;

    for (auto& scene : m_scenes)
        scene->update(timeDelta);
}

void GLRenderer::resize(int width, int height)
{
    bool hasReshaped = false;

    for (auto& scene : m_scenes)
    {
        hasReshaped = scene->reshape(width, height);
        if (hasReshaped == true)
            break;
    }

    if (hasReshaped == false)
    {
        float resWidth = static_cast<float>(width);
        float resHeight = static_cast<float>(height);
        float newRatio = resWidth / resHeight;

        float newWidth;
        float newHeight;

        if (newRatio > m_aspectRatio)
        {
            newWidth = resHeight * m_aspectRatio;
            newHeight = resHeight;
        }
        else
        {
            newHeight = resWidth / m_aspectRatio;
            newWidth = resWidth;
        }

        GLint x = static_cast<GLint>((resWidth - newWidth) / 2);
        GLint y = static_cast<GLint>((resHeight - newHeight) / 2);

        glViewport(x, y, static_cast<GLsizei>(newWidth), static_cast<GLsizei>(newHeight));
    }
}