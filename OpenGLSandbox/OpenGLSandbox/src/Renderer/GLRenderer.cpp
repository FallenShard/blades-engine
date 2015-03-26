#include "Renderer/GLRenderer.h"
#include "Input/Event.h"

#include "OglWrapper/FrameBuffer.h"
#include "OglWrapper/Texture.h"

#include <iostream>

namespace
{
    bool useFXAA = true;
}

void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id,
    GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    std::cout << "---------------------opengl-callback-start------------" << std::endl;
    std::cout << "message: " << message << std::endl;
    std::cout << "type: ";
    switch (type) {
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

    std::cout << "id: " << id << std::endl;    std::cout << "severity: ";
    switch (severity){
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
    std::cout << "---------------------opengl-callback-end--------------" << std::endl;
}

namespace fsi
{

GLRenderer::GLRenderer(Window* window)
    : m_aspectRatio(-1.f)
    , m_timePassed(0.f)
    , m_window(window)
    , m_shaderManager(nullptr)
    , m_aaPass(nullptr)
    , m_sceneManager(nullptr)
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
    delete m_sceneManager;
    delete m_aaPass;
}

void GLRenderer::init()
{
    glDebugMessageCallback(DebugCallback, NULL);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);


    if (m_window)
    {
        glm::ivec2 windowSize = m_window->getSize();
        m_aspectRatio = static_cast<float>(windowSize.x) / windowSize.y;
    }

    

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
    
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
    
    glClearColor(0.1f, 0.1f, 0.3f, 1.f);
    glClearDepth(1.0f);

    m_shaderManager = new ShaderManager();

    m_sceneManager = new SceneManager(m_window, m_shaderManager);
    m_sceneManager->prepare();

    int maxUnits;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxUnits);
    std::cout << "Texture units: " << maxUnits << std::endl;

    glm::ivec2 windowSize = m_window->getSize();

    m_aaPass = new RenderPass();
    m_aaPass->attachProgram(m_shaderManager->getProgram("fxaa"));
    m_aaPass->init(windowSize.x, windowSize.y);
}

void GLRenderer::draw()
{
    if (useFXAA)
    {
        m_aaPass->activate();

        m_sceneManager->render();
        
        FrameBuffer::bindScreen();
        FrameBuffer::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_aaPass->render();
    }
    else
    {
        FrameBuffer::bindScreen();
        FrameBuffer::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_sceneManager->render();
    }
}

void GLRenderer::handleEvents(const Event& event)
{
    if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space)
        useFXAA = !useFXAA;

    if (event.type == Event::Resized)
        m_aaPass->resize(event.size.width, event.size.height);

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
    
}

}