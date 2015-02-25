#include "Renderer/GLRenderer.h"
#include "Window/Event.h"

#include "OglWrapper/FrameBuffer.h"
#include "OglWrapper/Texture.h"

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
    , m_scene(nullptr)
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
    delete m_scene;
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

    m_scene = new Scene(m_window, m_shaderManager);
    m_scene->prepare();

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

        m_scene->render();
        
        FrameBuffer::bindScreen();
        FrameBuffer::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_aaPass->render();
    }
    else
    {
        FrameBuffer::bindScreen();
        FrameBuffer::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_scene->render();
    }
}

void GLRenderer::handleEvents(const Event& event)
{
    if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space)
        useFXAA = !useFXAA;

    if (event.type == Event::Resized)
        m_aaPass->resize(event.size.width, event.size.height);

    m_scene->handleEvents(event);
}

void GLRenderer::update(float timeDelta)
{
    m_timePassed += timeDelta;

    m_scene->update(timeDelta);
}

void GLRenderer::resize(int width, int height)
{
    m_scene->reshape(width, height);
    
}