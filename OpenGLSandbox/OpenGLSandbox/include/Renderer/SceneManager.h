#pragma once

#include "Models/PlaneGrid.h"
#include "Models/SceneNode.h"
#include "Models/TransformNode.h"
#include "Models/Cube.h"
#include "Models/RoboticArm.h"
#include "Models/Sphere.h"
#include "Camera/FreeCamera.h"
#include "Camera/CameraController.h"
#include "Renderer/ShaderManager.h"
#include "Window/Window.h"

#include <vector>

namespace fsi
{

class SceneManager
{
public:
    SceneManager();
    SceneManager(Window* window, ShaderManager* shaderManager);
    ~SceneManager();

    void prepare();
    void handleEvents(const Event& event);
    void update(float timeDelta);
    void render();

    bool reshape(int width, int height);

private:
    float m_timePassed;

    CameraController m_cameraController;
    FreeCamera m_freeCamera;

    ShaderManager* m_shaderManager;
    //MeshManager* m_meshManager;
    SceneNode* m_sceneGraph;

    std::vector<SceneNode*> m_validationVector;

    PlaneGrid*  m_planeGrid;
    RoboticArm* m_roboticArm;
    Cube*      m_Cube;
    Sphere*     m_sphere;
};

}