#include "Renderer/SceneManager.h"

#include "Models/SceneNode.h"
#include "Models/TransformNode.h"
#include "Models/Cube.h"
#include "Models/RoboticArm.h"
#include "Models/Sphere.h"

#include "Renderer/ShaderManager.h"

#include "Materials/Material.h"
#include "Materials/PhongMaterial.h"

#include <algorithm>

#include "Models/Terrain.h"

namespace fsi
{
    namespace
    {
        SphereMesh* g_sphMesh = nullptr;
        CubeMesh* g_prMesh = nullptr;

        ShaderProgram* prog = nullptr;


        GLenum glError;

        PhongMaterial* g_mat;
        PhongMaterial* g_mat2;

        Sphere** g_spheres;

        float shininess = 100.f;

        Terrain* g_terrain;
    }
}

namespace fsi
{

SceneManager::SceneManager()
    : m_timePassed(0.f)
    , m_planeGrid(new PlaneGrid(1000.f, 10.f, PlaneGrid::XZ))
    , m_cameraController(nullptr)
{
}

SceneManager::SceneManager(Window* window, ShaderManager* shaderManager)
    : m_timePassed(0.f)
    , m_planeGrid(new PlaneGrid(1000.f, 10.f, PlaneGrid::XZ))
    , m_cameraController(window)
    , m_shaderManager(shaderManager)
    , m_sceneGraph(new TransformNode())
{
}

SceneManager::~SceneManager()
{
    delete m_sceneGraph;
    delete g_sphMesh;
    delete g_prMesh;
    delete g_mat;
    delete g_mat2;

    delete g_terrain;
}

void SceneManager::prepare()
{
    prog = m_shaderManager->getProgram("phong");

    m_cameraController.setPosition(glm::vec3(0.f, 32.f, 36.f));

    m_planeGrid->setProgram(prog);
    m_sceneGraph->attachChild(m_planeGrid);

    g_prMesh = new CubeMesh();

    m_roboticArm = new RoboticArm(g_prMesh, prog, &m_validationVector);

    m_sceneGraph->attachChild(m_roboticArm);
    
    m_Cube = new Cube(g_prMesh, prog);
    m_sceneGraph->attachChild(m_Cube);
    m_Cube->setScale(glm::vec3(1.5f, 1.5f, 1.5f));

    g_sphMesh = new SphereMesh(2.f, 16, 32);

    m_sphere = new Sphere(g_sphMesh, prog);
    m_sphere->translate(glm::vec3(10.f, 0.f, -10.f));

    g_mat = new PhongMaterial(glm::vec4(1.f, 0.f, 1.f, 1.f),
                              glm::vec4(1.f, 0.f, 1.f, 1.f),
                              glm::vec4(1.f, 0.f, 1.f, 1.f),
                              shininess);
    g_mat->setShaderProgram(prog);

    g_mat2 = new PhongMaterial(glm::vec4(0.f, 0.f, 1.f, 1.f),
        glm::vec4(0.f, 0.f, 1.f, 1.f),
        glm::vec4(1.f, 1.f, 1.f, 1.f),
        shininess);
    g_mat2->setShaderProgram(prog);

    Sphere* smallSphere = new Sphere(g_sphMesh, prog, g_mat);
    smallSphere->setScale(glm::vec3(0.5f, 0.5f, 0.5f));
    smallSphere->translate(glm::vec3(0.f, 5.f, 0.f));
    m_sphere->attachChild(smallSphere);

    g_spheres = new Sphere*[10];
    for (int i = 0; i < 10; i++)
    {
        g_spheres[i] = new Sphere(g_sphMesh, prog);
        g_spheres[i]->setScale(glm::vec3(1.f + i * 0.5f, 1.f + i * 0.5f, 1.f + i * 0.5f));
        g_spheres[i]->translate(glm::vec3(10.f * i , i * i * sin(i * 3), i * i  * cos(i * 3)));
        m_sceneGraph->attachChild(g_spheres[i]);
    }

    m_sceneGraph->attachChild(m_sphere);


    g_terrain = new Terrain();
    g_terrain->init();
    

    m_validationVector.push_back(m_sceneGraph);
}

void SceneManager::handleEvents(const Event& event)
{
    m_cameraController.handleEvents(event);

    if (event.type == Event::KeyPressed)
        if (event.key.code == Keyboard::B)
            g_mat->setAmbientColor(glm::vec4(0.f, 0.f, 1.f, 1.f));
}

void SceneManager::update(float timeDelta)
{
    m_timePassed += timeDelta;

    m_cameraController.update(timeDelta);

    m_roboticArm->update(timeDelta);
    g_terrain->update(timeDelta);
    //m_validationVector.push_back(m_roboticArm);

    if (Keyboard::isKeyPressed(Keyboard::I))
    {
        m_sphere->translate(glm::vec3(-5.f * timeDelta, 0.f, 0.f));
        m_validationVector.push_back(m_sphere);
    }

    if (Keyboard::isKeyPressed(Keyboard::Add))
    {
        g_mat->setShininess(shininess += 5.f);
    }

    if (Keyboard::isKeyPressed(Keyboard::Subtract))
    {
        g_mat->setShininess(shininess -= 5.f);
    }

    if (Keyboard::isKeyPressed(Keyboard::O))
    {
        m_sphere->translate(glm::vec3(5.f * timeDelta, 0.f, 0.f));
        m_validationVector.push_back(m_sphere);
    }

    if (Keyboard::isKeyPressed(Keyboard::NumPad8))
    {
        m_Cube->translate(glm::vec3(0.f, 0.f, -0.16f));
        m_validationVector.push_back(m_Cube);
    }

    if (Keyboard::isKeyPressed(Keyboard::NumPad5))
    {
        m_Cube->translate(glm::vec3(0.f, 0.f, 0.16f));
        m_validationVector.push_back(m_Cube);
    }

    if (Keyboard::isKeyPressed(Keyboard::NumPad4))
    {
        m_Cube->translate(glm::vec3(-0.16f, 0.f, 0.f));
        m_validationVector.push_back(m_Cube);
    }

    if (Keyboard::isKeyPressed(Keyboard::NumPad6))
    {
        m_Cube->translate(glm::vec3(0.16f, 0.f, 0.f));
        m_validationVector.push_back(m_Cube);
    }

    if (Keyboard::isKeyPressed(Keyboard::NumPad7))
    {
        m_Cube->rotateY(1.5f);
        m_validationVector.push_back(m_Cube);
    }

    if (Keyboard::isKeyPressed(Keyboard::NumPad9))
    {
        m_Cube->rotateY(-1.5f);
        m_validationVector.push_back(m_Cube);
    }

    for (SceneNode* node : m_validationVector)
    {
        SceneNode* nodeToValidate = node->getTopLevelInvalidated();
        nodeToValidate->validate(timeDelta);
    }

    m_validationVector.clear();
}

void SceneManager::render()
{
    prog->use();

    prog->setUniformAttribute("mvLightPos", m_cameraController.getViewMatrix() * glm::vec4(0.f, 10.f, -10.f, 1.f));
    prog->setUniformAttribute("mvCameraPos", m_cameraController.getCameraPosition());
    glLineWidth(3.f);
    
    g_mat2->apply();
    m_sceneGraph->render(m_cameraController.getProjectionMatrix(), m_cameraController.getViewMatrix());

    g_terrain->render(m_cameraController.getProjectionMatrix(), m_cameraController.getViewMatrix());
    //glEnable(GL_POLYGON_OFFSET_LINE);
    //glPolygonOffset(-1, -1);

    // draw the wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //draw the same polygons again
    //m_sceneGraph->render(m_cameraController.getProjectionMatrix(), m_cameraController.getViewMatrix());

    // restore default polygon mode
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

bool SceneManager::reshape(int width, int height)
{
    m_cameraController.resize(width, height);
    return true;
}

}