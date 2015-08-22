#pragma once

#include <vector>

#include "Camera/CameraController.h"

namespace fsi
{
    class Window;
    class ShaderManager;
    class Terrain;
    class Sphere;
    class Skybox;

    class Scene
    {
    public:
        Scene(std::shared_ptr<Window>& window, ShaderManager* shaderManager);
        ~Scene();

        void prepare();
        void handleEvents(const Event& event);
        void update(float timeDelta);
        void render();

        Terrain* getTerrain() const;

        bool reshape(int width, int height);

    private:
        float m_timePassed;

        CameraController m_cameraController;

        ShaderManager* m_shaderManager;

        Terrain* m_terrain;
        Sphere* m_sphere;
        Skybox* m_skybox;
    };
}