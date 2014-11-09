#pragma once

#include "Scene.h"

#include "Models/PlaneGrid.h"
#include "Models/SceneNode.h"
#include "Models/TransformNode.h"
#include "Models/Prism.h"
#include "Models/RoboticArm.h"
#include "Models/Sphere.h"
#include "Renderer/FreeCamera.h"
#include "Renderer/CameraController.h"
#include "Renderer/ShaderManager.h"
#include "Window/Window.h"

class Scene
{
public:
    Scene();
    Scene(Window* window, ShaderManager* shaderManager);
    ~Scene();

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

    PlaneGrid*  m_planeGrid;
    RoboticArm* m_roboticArm;
    Prism*      m_prism;
    Sphere*     m_sphere;
};