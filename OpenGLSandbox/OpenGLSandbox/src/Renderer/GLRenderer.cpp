#include <iostream>
#include <iomanip>
#include <chrono>
#include <fstream>
#include <functional>

#include "PostProcessing/RenderPass.h"
#include "PostProcessing/FXAA.h"

#include "Renderer/GLRenderer.h"
#include "Renderer/UIRenderer.h"
#include "Input/Event.h"

#include "Core/Timer.h"

#include "Renderer/GLContext.h"
#include "Renderer/TechniqueCache.h"
#include "Renderer/TextureManager.h"
#include "Renderer/DeviceBufferManager.h"
#include "Renderer/VertexAssembly.h"
#include "Renderer/FramebufferManager.h"

#include "Renderer/DrawItem.h"

namespace
{
    fsi::Timer fpsTimer;

    GLuint fxaa;
    GLuint terr;
    GLuint color;
    GLuint phong;
    GLuint simple;
    GLuint gui;
    GLuint text;
    GLuint sky;

    // Terrain parameters
    GLint heightMap;
    GLuint heightTex;
    GLuint heightSampler;

    GLint fineMap;
    GLuint fineTex;
    GLuint fineSampler;

    GLint detailMap;
    GLuint dirtTex;
    GLuint dirtSampler;

    GLint noiseMap;
    GLint detMap;
    GLuint noiseTex;
    GLuint detTex;

    GLuint timeUnif;
    GLfloat timeVal = 0.f;

    GLint detail = 1;

    int m_tiles = 128;
    float m_tileSize = 1.f;
    float m_worldScale = 24.f;
    float m_heightFactor = m_tiles / 16.f;
    float m_terrainSize = m_tiles * m_tileSize * m_worldScale;
    float m_triSize = 8.f;

    int vertCount = 0;
    int indCount = 0;

    std::vector<GLfloat> m_vertices;
    std::vector<GLushort> m_indices;

    GLuint lineVao;
    GLuint lineVbo;
    std::vector<GLfloat> lineVerts;

    GLuint terrVao;
    GLuint terrVbo;
    GLuint terrIbo;
    GLuint terrUbo;

    void fillOriginBuffer()
    {
        // X
        lineVerts.push_back(0.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(1.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(1.f);
        lineVerts.push_back(10.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(1.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(1.f);

        // Y
        lineVerts.push_back(0.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(1.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(1.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(10.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(1.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(1.f);

        // Z
        lineVerts.push_back(0.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(1.f);
        lineVerts.push_back(1.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(10.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(0.f);
        lineVerts.push_back(1.f);
        lineVerts.push_back(1.f);
    }

    GLuint aaVao;
    GLuint aaFbo;
    GLuint aaTex;
    GLuint aaSampler;
    GLuint aaRbo;
    GLuint aaTexUnit = 1;

    struct TerrainParams
    {
        float detailFactor;
        float heightFactor;
        float patches;
        float terrainSize;
    };

    TerrainParams terrParams;
    fsi::Technique* t;
    fsi::Technique* colTech;

    std::vector<fsi::DrawItem> drawItems(2);
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
        , m_scene(nullptr)
        , m_uiRenderer(nullptr)
        , m_FXAAenabled(true)
        , m_showGui(true)
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

        // Shader resources
        fxaa = m_techniqueCache->getProgram("fxaa");
        terr = m_techniqueCache->getProgram("terrain");
        phong = m_techniqueCache->getProgram("phong");
        color = m_techniqueCache->getProgram("color");
        text = m_techniqueCache->getProgram("text");
        simple = m_techniqueCache->getProgram("simple");
        gui = m_techniqueCache->getProgram("gui");
        sky = m_techniqueCache->getProgram("skybox");

        heightTex = m_textureManager->loadTexture("heightmap1024.png", 1, InternalFormat::R8, BaseFormat::Red);
        fineTex = m_textureManager->loadTexture("perlinHmap.png", 1, InternalFormat::R8, BaseFormat::Red);
        dirtTex = m_textureManager->loadTexture("lunar.png", 8, InternalFormat::RGBA8, BaseFormat::RGBA);
        noiseTex = m_textureManager->loadTexture("noise.png", 8, InternalFormat::R8, BaseFormat::Red);
        detTex = m_textureManager->loadTexture("seamlessmoon.png", 8, InternalFormat::RGBA8, BaseFormat::RGBA);

        heightSampler = m_textureManager->getSamplerPreset(TextureManager::LinearClamp);
        fineSampler = m_textureManager->getSamplerPreset(TextureManager::LinearMirrored);
        dirtSampler = m_textureManager->createSampler(Filter::Mipmap, Filter::Linear, WrapMode::Repeat, WrapMode::Repeat);

        auto heightTexInfo = m_textureManager->createTextureInfo(heightTex, heightSampler);
        auto detailTexInfo = m_textureManager->createTextureInfo(fineTex, fineSampler);
        auto globalTexInfo = m_textureManager->createTextureInfo(dirtTex, dirtSampler);
        auto noiseTexInfo = m_textureManager->createTextureInfo(noiseTex, dirtSampler);
        auto patchTexInfo = m_textureManager->createTextureInfo(detTex, dirtSampler);

        

        // 3D Scene manager
        //m_Scene = new Scene(m_window, m_shaderManager);

        //m_Scene->prepare();

        // TERRAIN SETUP
        for (int z = 0; z < m_tiles + 1; z++)
        {
            for (int x = 0; x < m_tiles + 1; x++)
            {
                m_vertices.push_back(x * m_tileSize - m_tiles * m_tileSize / 2.f);
                m_vertices.push_back(0.f);
                m_vertices.push_back(m_tiles * m_tileSize / 2.f - z * m_tileSize);
            }
        }

        for (int z = 0; z < m_tiles; z++)
        {
            for (int x = 0; x < m_tiles; x++)
            {
                m_indices.push_back(z * (m_tiles + 1) + x);
                m_indices.push_back(z * (m_tiles + 1) + x + 1);
                m_indices.push_back((z + 1) * (m_tiles + 1) + x + 1);
                m_indices.push_back((z + 1) * (m_tiles + 1) + x);
            }
        }

        vertCount = m_vertices.size() / 3;

        indCount = m_indices.size();

        terrVbo = m_deviceBufferManager->allocate(m_vertices.size() * sizeof(GLfloat), GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
        m_deviceBufferManager->update(terrVbo, m_vertices, 0);
        terrIbo = m_deviceBufferManager->allocate(m_indices.size() * sizeof(GLushort), GL_MAP_WRITE_BIT);
        m_deviceBufferManager->update(terrIbo, m_indices, 0);

        VertexLayout layout;
        layout.indexBuffer = terrIbo;
        layout.vertexBuffers.emplace_back(0, BufferDescriptor{ terrVbo, 0, 3 * sizeof(GLfloat) });
        layout.attributes.emplace_back(0, AttributeFormat{ VertexAttrib::Position , 3, 0 });
        terrVao = m_vertexAssembly->createInputState(layout);

        glPatchParameteri(GL_PATCH_VERTICES, 4);

        t = new Technique(terr);
        t->setUniformAttribute("heightMap"      , heightTexInfo.unit);
        t->setUniformAttribute("detailHeightMap", detailTexInfo.unit);
        t->setUniformAttribute("terrainTexture" , globalTexInfo.unit);
        t->setUniformAttribute("noiseMap"       , noiseTexInfo.unit);
        t->setUniformAttribute("patchTexture"   , patchTexInfo.unit);
        
        t->setUniformAttribute("worldScale", m_worldScale);
        t->setUniformAttribute("triSize"   , m_triSize);
        t->setUniformAttribute("screenSize", glm::vec2(windowSize));
        t->setUniformAttribute("detail"    , detail);

        terrParams.heightFactor = m_heightFactor;
        terrParams.detailFactor = m_heightFactor * 0.0125f;
        terrParams.patches = static_cast<float>(m_tiles);
        terrParams.terrainSize = static_cast<float>(m_terrainSize);

        terrUbo = m_deviceBufferManager->allocate(sizeof(terrParams), GL_MAP_WRITE_BIT);
        m_deviceBufferManager->update(terrUbo, &terrParams, sizeof(terrParams));

        auto uboDesc = t->createUniformBufferDescriptor("TerrainParams", 0, terrUbo);

        drawItems[0].program = terr;
        drawItems[0].primitiveType = GL_PATCHES;
        drawItems[0].vertexArray = terrVao;
        drawItems[0].numVerts = vertCount / 3;
        drawItems[0].numIndices = indCount;
        drawItems[0].baseVertex = 0;
        drawItems[0].updateUniforms = [this](const glm::mat4& P, const glm::mat4& V)
        {
            t->setUniformAttribute("P", P);

            glm::mat4 model = glm::translate(glm::vec3(0.f, -m_worldScale * 10.f, 0.f)) * glm::scale(glm::vec3(m_worldScale));
            glm::mat4 mv = V * model;
            t->setUniformAttribute("MV", mv);

            glm::mat4 mat = P * mv;
            t->setUniformAttribute("MVP", mat);

            glm::mat3 normalMatrix = glm::mat3(glm::inverseTranspose(mv));
            t->setUniformAttribute("normalMatrix", normalMatrix);

            glm::vec4 lightDir = glm::normalize(V * glm::normalize(glm::vec4(1.f, 1.f, 0.f, 0.f)));
            t->setUniformAttribute("mvLightDir", lightDir);

            t->setUniformAttribute("wireframe", 0);
        };

        

        ///////////////////////////////////// Coord system
        fillOriginBuffer();

        lineVbo = m_deviceBufferManager->allocate(lineVerts.size() * sizeof(GLfloat), GL_MAP_WRITE_BIT);
        m_deviceBufferManager->update(lineVbo, lineVerts);

        VertexLayout lineLayout;
        lineLayout.indexBuffer = 0;
        lineLayout.vertexBuffers.emplace_back(0, BufferDescriptor{ lineVbo, 0, 7 * sizeof(GLfloat) });
        lineLayout.attributes.emplace_back(0, AttributeFormat{ VertexAttrib::Position, 3, 0 });
        lineLayout.attributes.emplace_back(0, AttributeFormat{ VertexAttrib::Color   , 4, 12 });

        lineVao = m_vertexAssembly->createInputState(lineLayout);

        colTech = new Technique(color);
        drawItems[1].program = color;
        drawItems[1].primitiveType = GL_LINES;
        drawItems[1].vertexArray = lineVao;
        drawItems[1].numVerts = 6;
        drawItems[1].numIndices = -1;
        drawItems[1].baseVertex = 0;
        drawItems[1].updateUniforms = [this](const glm::mat4& P, const glm::mat4& V)
        {
            colTech->use();
            glm::mat4 lModel(1.f);
            glm::mat4 lineMat = P * V * lModel;

            colTech->setUniformAttribute("MVP", lineMat);
        };

        // GUI manager
        //m_uiRenderer = std::make_shared<UIRenderer>(m_window, m_shaderManager, this, m_Scene);

        // FXAA Post-processing
        m_renderPasses.push_back(std::make_unique<FXAA>(windowSize.x, windowSize.y, this));
    }

    GLRenderer::~GLRenderer()
    {
        //delete m_Scene;
        //delete m_aaPass;
        //delete m_shaderManager;
        delete t;
        t = nullptr;

        delete colTech;
        colTech = nullptr;
    }

    void GLRenderer::enableDebugLogging()
    {
        glDebugMessageCallback(DebugCallback, nullptr);
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    }

    void GLRenderer::renderPriv(std::vector<DrawItem>& drawItems)
    {
        const glm::mat4 P = m_cameraController->getCamera()->getProjectionMatrix();
        const glm::mat4 V = m_cameraController->getCamera()->getViewMatrix();

        for (auto& item : drawItems)
        {
            glUseProgram(item.program);
            item.updateUniforms(P, V);

            glBindVertexArray(item.vertexArray);
            if (item.numIndices != -1)
                glDrawElementsBaseVertex(item.primitiveType, item.numIndices, GL_UNSIGNED_SHORT, nullptr, item.baseVertex);
            else
                glDrawArrays(item.primitiveType, 0, item.numVerts);
        }
    }

    void GLRenderer::render()
    {
        auto start = fpsTimer.getElapsedTime();

        float val[] = { 0.1f, 0.1f, 0.1f, 1.f };
        float depthVal = 1.f;

        glClearNamedFramebufferfv(0, GL_COLOR, 0, val);
        glClearNamedFramebufferfv(0, GL_DEPTH, 0, &depthVal);

        // TODO
        auto currentPass = m_renderPasses.begin();
        
        if (currentPass != m_renderPasses.end())
            (*currentPass)->setAsSurface();
        else
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

        renderPriv(drawItems);
        for (; currentPass != m_renderPasses.end(); ++currentPass)
        {
            auto nextPass = std::next(currentPass);
            if (nextPass != m_renderPasses.end())
                (*nextPass)->setAsSurface();
            else
                glBindFramebuffer(GL_FRAMEBUFFER, 0);

            (*currentPass)->render();
        }

        // Make sure not to render text/GUI under FXAA
        //if (m_showGui)
        //    m_uiRenderer->render();

        m_window->display();
    }

    void GLRenderer::handleEvents(const Event& event)
    {
        if (event.type == Event::KeyPressed && event.key.code == Keyboard::G)
            m_showGui = !m_showGui;

        if (event.type == Event::Resized)
        {
            //m_aaPass->resize(event.size.width, event.size.height);
            //m_uiRenderer->resize(event.size.width, event.size.height);
            resize(event.size.width, event.size.height);
        }

        //if (m_showGui)
        //    m_uiRenderer->handleEvents(event);

        //m_Scene->handleEvents(event);

        m_cameraController->handleEvents(event);
    }

    void GLRenderer::update(float timeDelta)
    {
        m_timePassed += timeDelta;

        //m_Scene->update(timeDelta);

        // Update camera
        m_cameraController->update(timeDelta);
    }

    void GLRenderer::setFrameTime(long long frameTime)
    {
        //m_uiRenderer->setFrameTime(frameTime);
    }

    void GLRenderer::resize(int width, int height)
    {
        //m_Scene->reshape(width, height);

        m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);

        for (auto& pass : m_renderPasses)
            pass->resize(width, height);
        //m_uiRenderer->resize(width, height);
    }

    void GLRenderer::enableFXAA(bool enabled)
    {
        m_FXAAenabled = enabled;
    }
}