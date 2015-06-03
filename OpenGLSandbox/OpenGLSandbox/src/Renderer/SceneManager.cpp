#include "Renderer/SceneManager.h"
#include "Renderer/ShaderManager.h"

#include "Models/Terrain.h"
#include "Models/Sphere.h"

namespace fsi
{

SceneManager::SceneManager(Window* window, ShaderManager* shaderManager)
    : m_timePassed(0.f)
    , m_cameraController(window)
    , m_shaderManager(shaderManager)
{
}

SceneManager::~SceneManager()
{
    delete m_terrain;
    delete m_sphere;
}

void SceneManager::prepare()
{
    m_sphere = new Sphere(m_shaderManager->getProgram("phong"));
    m_terrain = new Terrain(m_shaderManager);
}

void SceneManager::handleEvents(const Event& event)
{
    m_cameraController.handleEvents(event);
}

void SceneManager::update(float timeDelta)
{
    m_timePassed += timeDelta;

    m_cameraController.update(timeDelta);

    m_terrain->update(timeDelta);
}

void SceneManager::render()
{
    m_sphere->render(m_cameraController.getProjectionMatrix(), m_cameraController.getViewMatrix());
    m_terrain->render(m_cameraController.getProjectionMatrix(), m_cameraController.getViewMatrix());
}

bool SceneManager::reshape(int width, int height)
{
    m_cameraController.resize(width, height);
    return true;
}

}