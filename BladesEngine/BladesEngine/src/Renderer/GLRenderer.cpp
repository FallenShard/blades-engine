#include <iostream>
#include <iomanip>
#include <chrono>
#include <fstream>
#include <functional>

#include "Core/Timer.h"

#include "PostProcessing/RenderPass.h"
#include "PostProcessing/FXAA.h"

#include "Renderer/GLContext.h"
#include "Renderer/GLRenderer.h"
#include "GUI/GUIManager.h"
#include "Input/Event.h"

#include "Models/Terrain.h"
#include "Models/CoordOrigin.h"
#include "Models/Skybox.h"
#include "Models/Sphere.h"

namespace
{
    fsi::Timer fpsTimer;

    fsi::Terrain* terrain = nullptr;
    fsi::CoordOrigin* origin = nullptr;
    fsi::Skybox* skybox = nullptr;
    fsi::Sphere* sphere = nullptr;
}

void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id,
    GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    if (type == GL_DEBUG_TYPE_OTHER)
        return;
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
        , m_glContext(std::make_shared<GLContext>(m_window))
        , m_techniqueCache(std::make_shared<TechniqueCache>())
        , m_textureManager(std::make_shared<TextureManager>())
        , m_deviceBufferManager(std::make_shared<DeviceBufferManager>())
        , m_vertexAssembly(std::make_shared<VertexAssembly>())
        , m_framebufferManager(std::make_shared<FramebufferManager>())
        , m_cameraController(std::make_shared<CameraController>(m_window))
        , m_guiManager(nullptr)
        , m_FXAAenabled(true)
        , m_showGui(true)
        , m_isWireframe(false)
    {
#ifdef _DEBUG
        enableDebugLogging();
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
        glDepthRange(0.f, 1.f);

        glClearColor(0.1f, 0.1f, 0.1f, 1.f);
        glClearDepth(1.f);

        // 3D Scene manager
        skybox = new Skybox(5, this);
        terrain = new Terrain(128, 1.f, 24.f, this);
        origin = new CoordOrigin(10, this);
        sphere = new Sphere(3.f, 32, 32, this);

        // GUI manager
        m_guiManager = std::make_shared<gui::GUIManager>(m_window, this, nullptr);

        // FXAA Post-processing
        m_renderPasses.push_back(std::make_unique<FXAA>(windowSize.x, windowSize.y, this));
    }

    GLRenderer::~GLRenderer()
    {
        delete origin;
        origin = nullptr;

        delete terrain;
        terrain = nullptr;

        delete skybox;
        skybox = nullptr;

        delete sphere;
        sphere = nullptr;
    }

    void GLRenderer::enableDebugLogging()
    {
        glDebugMessageCallback(DebugCallback, nullptr);
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    }

    void GLRenderer::renderScene()
    {
        const glm::mat4 P = m_cameraController->getCamera()->getProjectionMatrix();
        const glm::mat4 V = m_cameraController->getCamera()->getViewMatrix();

        for (auto& item : m_drawItems)
        {
            glUseProgram(item.program);
            
            item.preRender(P, V);

            glBindVertexArray(item.vertexArray);
            if (item.numIndices != -1)
                glDrawElementsBaseVertex(item.primitiveType, item.numIndices, GL_UNSIGNED_SHORT, nullptr, item.baseVertex);
            else
                glDrawArrays(item.primitiveType, 0, item.numVerts);

            if (item.postRender) item.postRender();
        }
    }

    void GLRenderer::render()
    {
        auto start = fpsTimer.getElapsedTime();

        RenderPass::clearScreen();

        auto currentPass = m_renderPasses.begin();
        
        if (currentPass != m_renderPasses.end())
            (*currentPass)->setAsSurface();
        else
            RenderPass::setScreenAsSurface();
        if (m_isWireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        renderScene();

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        for (; currentPass != m_renderPasses.end(); ++currentPass)
        {
            auto nextPass = std::next(currentPass);
            if (nextPass != m_renderPasses.end())
                (*nextPass)->setAsSurface();
            else
                RenderPass::setScreenAsSurface();

            (*currentPass)->render();
        }

        // Make sure not to render text/GUI under FXAA
        if (m_showGui)
            m_guiManager->render();

        m_window->display();
    }

    void GLRenderer::handleEvents(const Event& event)
    {
        m_cameraController->handleEvents(event);

        if (event.type == Event::KeyPressed && event.key.code == Keyboard::G)
            m_showGui = !m_showGui;

        if (event.type == Event::Resized)
        {
            resize(event.size.width, event.size.height);
            return;
        }

        if (m_showGui)
            m_guiManager->handleEvents(event);

        //m_Scene->handleEvents(event);
    }

    void GLRenderer::update(float timeDelta)
    {
        m_timePassed += timeDelta;

        //m_Scene->update(timeDelta);

        // Update camera
        m_cameraController->update(timeDelta);

        skybox->update(m_cameraController->getCamera()->getPosition());
    }

    void GLRenderer::setFrameTime(long long frameTime)
    {
        m_guiManager->setFrameTime(frameTime);
    }

    void GLRenderer::resize(int width, int height)
    {
        m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);

        for (auto& pass : m_renderPasses)
            pass->resize(width, height);
        
        m_guiManager->resize(width, height);
    }

    void GLRenderer::enableFXAA(bool enabled)
    {
        m_FXAAenabled = enabled;
    }

    void GLRenderer::setWireframeMode(bool isWireframe)
    {
        m_isWireframe = isWireframe;
    }

    void GLRenderer::submitDrawItem(const DrawItem& drawItem)
    {
        m_drawItems.emplace_back(drawItem);
    }

    void GLRenderer::submitGuiDrawItem(const DrawItem& drawItem)
    {
        m_guiDrawItems.emplace_back(drawItem);
    }

    void GLRenderer::setTessellationPatchVertices(int numberOfVertices)
    {
        glPatchParameteri(GL_PATCH_VERTICES, numberOfVertices);
    }

    glm::ivec2 GLRenderer::getScreenSize() const
    {
        return m_window->getSize();
    }
}