#include "Renderer/Scene.h"

#include "Models/SceneNode.h"
#include "Models/TransformNode.h"
#include "Models/Prism.h"
#include "Models/RoboticArm.h"
#include "Models/Sphere.h"

#include "Renderer/ShaderManager.h"

#include <algorithm>

namespace
{
    SphereMesh* g_sphMesh = nullptr;
    PrismMesh* g_prMesh = nullptr;

    ShaderProgram* prog = nullptr;
}

Scene::Scene()
    : m_timePassed(0.f)
    , m_planeGrid(new PlaneGrid(1000.f, 10.f, PlaneGrid::XZ))
    , m_cameraController(nullptr, nullptr)
{
}

Scene::Scene(Window* window, ShaderManager* shaderManager)
    : m_timePassed(0.f)
    , m_planeGrid(new PlaneGrid(1000.f, 10.f, PlaneGrid::XZ))
    , m_cameraController(nullptr, window)
    , m_shaderManager(shaderManager)
    , m_sceneGraph(new TransformNode())
{
}

Scene::~Scene()
{
    delete m_sceneGraph;
    delete g_sphMesh;
    delete g_prMesh;
}

void Scene::prepare()
{
    prog = m_shaderManager->getProgram("perspective");

    prog->getUniformAttribute("MVP");
    prog->getUniformAttribute("MV");
    prog->getUniformAttribute("color");

    m_cameraController.setShaderProgram(prog);
    m_cameraController.setPosition(glm::vec3(0.f, 32.f, 36.f));

    m_planeGrid->setProgram(prog);
    m_sceneGraph->attachChild(m_planeGrid);

    g_prMesh = new PrismMesh();
    m_roboticArm = new RoboticArm(g_prMesh, prog);
    m_sceneGraph->attachChild(m_roboticArm);
    
    m_prism = new Prism(g_prMesh, prog);
    m_sceneGraph->attachChild(m_prism);
    m_prism->setScale(glm::vec3(1.5f, 1.5f, 1.5f));

    g_sphMesh = new SphereMesh(2.f, 16, 32);

    m_sphere = new Sphere(g_sphMesh, prog);
    m_sphere->translate(glm::vec3(10.f, 0.f, -10.f));
    m_sceneGraph->attachChild(m_sphere);
}

void Scene::handleEvents(const Event& event)
{
    m_cameraController.handleEvents(event);
}

void Scene::update(float timeDelta)
{
    m_timePassed += timeDelta;

    m_cameraController.update(timeDelta);

    m_roboticArm->update(timeDelta);

    if (Keyboard::isKeyPressed(Keyboard::I))
    {
        m_sphere->translate(glm::vec3(-5.f * timeDelta, 0.f, 0.f));
    }

    if (Keyboard::isKeyPressed(Keyboard::O))
    {
        m_sphere->translate(glm::vec3(5.f * timeDelta, 0.f, 0.f));
    }

    if (Keyboard::isKeyPressed(Keyboard::NumPad8))
    {
        m_prism->translate(glm::vec3(0.f, 0.f, -0.16f));
    }

    if (Keyboard::isKeyPressed(Keyboard::NumPad5))
    {
        m_prism->translate(glm::vec3(0.f, 0.f, 0.16f));
    }

    if (Keyboard::isKeyPressed(Keyboard::NumPad4))
    {
        m_prism->translate(glm::vec3(-0.16f, 0.f, 0.f));
    }

    if (Keyboard::isKeyPressed(Keyboard::NumPad6))
    {
        m_prism->translate(glm::vec3(0.16f, 0.f, 0.f));
    }

    if (Keyboard::isKeyPressed(Keyboard::NumPad7))
    {
        m_prism->rotateY(1.5f);
    }

    if (Keyboard::isKeyPressed(Keyboard::NumPad9))
    {
        m_prism->rotateY(-1.5f);
    }
}

void Scene::render()
{
    prog->use();
    //prog->setUniformAttribute("modelToWorldMatrix", glm::mat4(1.f));

    m_sceneGraph->render(m_cameraController.getProjectionMatrix(), m_cameraController.getViewMatrix());
}

bool Scene::reshape(int width, int height)
{
    m_cameraController.resize(width, height);
    return true;
}