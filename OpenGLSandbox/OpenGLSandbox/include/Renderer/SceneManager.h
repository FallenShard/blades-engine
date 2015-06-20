#pragma once

#include <vector>


#include "Camera/FreeCamera.h"
#include "Camera/CameraController.h"

namespace fsi
{

class Window;
class ShaderManager;
class Terrain;
class Sphere;
class Skybox;

class SceneManager
{
public:
    SceneManager(std::shared_ptr<Window>& window, ShaderManager* shaderManager);
    ~SceneManager();

    void prepare();
    void handleEvents(const Event& event);
    void update(float timeDelta);
    void render();

    Terrain* getTerrain() const;

    bool reshape(int width, int height);

private:
    float m_timePassed;

    CameraController m_cameraController;
    FreeCamera m_freeCamera;

    ShaderManager* m_shaderManager;

    Terrain* m_terrain;
    Sphere* m_sphere;
    Skybox* m_skybox;
};

}