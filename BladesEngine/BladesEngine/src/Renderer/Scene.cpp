#include "Renderer/Scene.h"

#include "Models/Terrain.h"
#include "Models/Sphere.h"
#include "Models/Skybox.h"

namespace fsi
{
    Scene::Scene(std::shared_ptr<Window>& window, ShaderManager* shaderManager)
        : m_timePassed(0.f)
        , m_cameraController(window)
        , m_shaderManager(shaderManager)
    {
    }

    Scene::~Scene()
    {
        delete m_terrain;
        delete m_sphere;
    }

    void Scene::prepare()
    {
        //m_sphere = new Sphere(m_shaderManager->getProgram("phong"));
        //m_terrain = new Terrain(m_shaderManager);
        //m_skybox = new Skybox(m_shaderManager->getProgram("skybox"));
    }

    void Scene::handleEvents(const Event& event)
    {
        m_cameraController.handleEvents(event);
    }

    void Scene::update(float timeDelta)
    {
        m_timePassed += timeDelta;

        m_cameraController.update(timeDelta);
        m_skybox->update(m_cameraController.getCamera()->getPosition());
        //m_terrain->update(timeDelta);
    }

    void Scene::render()
    {
        //m_skybox->render(m_cameraController.getProjectionMatrix(), m_cameraController.getViewMatrix());
        //m_sphere->render(m_cameraController.getProjectionMatrix(), m_cameraController.getViewMatrix());
        //m_terrain->render(m_cameraController.getProjectionMatrix(), m_cameraController.getViewMatrix());
    }

    bool Scene::reshape(int width, int height)
    {
        m_cameraController.resize(width, height);
        return true;
    }

    Terrain* Scene::getTerrain() const
    {
        return m_terrain;
    }
}