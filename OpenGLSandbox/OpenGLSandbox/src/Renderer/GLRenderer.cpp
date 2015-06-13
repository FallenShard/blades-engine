#include "Renderer/GLRenderer.h"
#include "Renderer/UIRenderer.h"
#include "Input/Event.h"

#include <iostream>
#include <iomanip>

namespace
{
    bool useFXAA = true;
}

void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id,
    GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    std::cout << std::left << std::endl << "----------OpenGL Debug Callback Start----------" << std::endl;
    std::cout << std::setw(10) << "Message: " << message << std::endl;
    std::cout << std::setw(10) << "Type: ";
    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
        std::cout << "ERROR";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        std::cout << "DEPRECATED_BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        std::cout << "UNDEFINED_BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        std::cout << "PORTABILITY";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        std::cout << "PERFORMANCE";
        break;
    case GL_DEBUG_TYPE_OTHER:
        std::cout << "OTHER";
        break;
    }
    std::cout << std::endl;
    std::cout << std::setw(10) << "Id: " << id << std::endl;
    std::cout << std::setw(10) << "Severity: ";
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_LOW:
        std::cout << "LOW";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        std::cout << "MEDIUM";
        break;
    case GL_DEBUG_SEVERITY_HIGH:
        std::cout << "HIGH";
        break;
    }
    std::cout << std::endl;
    std::cout << "----------OpenGL Debug Callback End------------" << std::endl;
}

namespace fsi
{

GLRenderer::GLRenderer(std::shared_ptr<Window>& window)
    : m_aspectRatio(-1.f)
    , m_timePassed(0.f)
    , m_window(window)
    , m_shaderManager(nullptr)
    , m_aaPass(nullptr)
    , m_sceneManager(nullptr)
    , m_uiRenderer(nullptr)
{
    init();
}

GLRenderer::~GLRenderer()
{
    delete m_sceneManager;
    delete m_aaPass;
    delete m_shaderManager;
}

void GLRenderer::init()
{
#if defined(_DEBUG)
    glDebugMessageCallback(DebugCallback, nullptr);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#endif

    glm::ivec2 windowSize = m_window->getSize();
    m_aspectRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);

    // Enable culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    
    // Enable standard depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
    
    glClearColor(0.1f, 1.0f, 0.1f, 1.f);
    glClearDepth(1.0f);

    // Shader resources
    m_shaderManager = new ShaderManager();

    // 3D Scene manager
    m_sceneManager = new SceneManager(m_window, m_shaderManager);
    m_sceneManager->prepare();

    // GUI manager (TODO, it's just the text overlay for now)
    m_uiRenderer = std::make_shared<UIRenderer>(m_window, m_shaderManager);

    // FXAA Post-processing
    m_aaPass = new RenderPass(windowSize.x, windowSize.y, m_shaderManager->getProgram("fxaa"));
}

void GLRenderer::draw()
{
    if (useFXAA)
    {
        m_aaPass->activate();

        m_sceneManager->render();
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_aaPass->render();
    }
    else
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_sceneManager->render();
    }

    // Make sure not to render text/GUI under FXAA
    m_uiRenderer->render();
}

void GLRenderer::handleEvents(const Event& event)
{
    if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space)
        useFXAA = !useFXAA;

    if (event.type == Event::Resized)
    {
        m_aaPass->resize(event.size.width, event.size.height); 
        m_uiRenderer->resize(event.size.width, event.size.height);
    }
        
    m_uiRenderer->handleEvents(event);

    m_sceneManager->handleEvents(event);
}

void GLRenderer::update(float timeDelta)
{
    m_timePassed += timeDelta;

    m_sceneManager->update(timeDelta);
}

void GLRenderer::resize(int width, int height)
{
    m_sceneManager->reshape(width, height);
    m_uiRenderer->resize(width, height);
}

}