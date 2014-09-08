#include "Renderer/GLRenderer.h"
#include "Window/Event.h"

#include "Scenes/TriangleScene.h"
#include "Scenes/PrismScene.h"
#include "Scenes/OverlapScene.h"
#include "Scenes/TransformationScene.h"

GLRenderer::GLRenderer()
    : m_aspectRatio(4.f / 3)
    , m_timePassed(0.f)
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
}

void GLRenderer::init()
{
    Scene* scene = new PrismScene();
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

    glClearColor(0.f, 0.f, 0.3f, 1.f);
    glClearDepth(1.0f);
}

void GLRenderer::draw()
{
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto& scene : m_scenes)
        scene->render();

    VertexBuffer::release(GL_ARRAY_BUFFER);
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