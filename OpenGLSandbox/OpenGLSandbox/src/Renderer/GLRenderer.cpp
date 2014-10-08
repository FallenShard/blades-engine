#include "Renderer/GLRenderer.h"
#include "Window/Event.h"

#include "Scenes/TriangleScene.h"
#include "Scenes/PrismScene.h"
#include "Scenes/OverlapScene.h"
#include "Scenes/TranslationScene.h"
#include "Scenes/GraphScene.h"
#include "Scenes/RobotArmScene.h"

GLRenderer::GLRenderer(Window* window)
    : m_aspectRatio(-1.f)
    , m_timePassed(0.f)
    , m_window(window)
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
}

void GLRenderer::init()
{
    if (m_window)
    {
        glm::ivec2 windowSize = m_window->getSize();
        m_aspectRatio = static_cast<float>(windowSize.x) / windowSize.y;
    }
    //Scene* scene = new TriangleScene();
    //Scene* scene = new PrismScene();
    //Scene* scene = new OverlapScene();
    //Scene* scene = new GraphScene();
    //Scene* scene = new TranslationScene();
    Scene* scene = new RobotArmScene(m_window);
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
}

void GLRenderer::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto& scene : m_scenes)
        scene->render();

    VertexBuffer::release();
    VertexArray::release();
    ShaderProgram::release();
}

void GLRenderer::handleEvents(const Event& event)
{
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