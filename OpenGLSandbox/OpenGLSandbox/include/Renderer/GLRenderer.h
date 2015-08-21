#pragma once

#include <vector>
#include <map>
#include <list>

#include "OpenGL.h"
#include "Renderer/Scene.h"

#include "Window/Window.h"

namespace fsi
{
    struct Event;
    class GLContext;
    class UIRenderer;
    class TechniqueCache;
    class TextureManager;
    class DeviceBufferManager;
    class VertexAssembly;
    class FramebufferManager;
    class RenderPass;

    struct DrawItem;

    class GLRenderer
    {
    public:
        GLRenderer(std::shared_ptr<Window>& window);
        ~GLRenderer();

        void handleEvents(const Event& event);
        void update(float timeDelta);
        void render();

        void resize(int width, int height);

        void setFrameTime(long long frameTime);

        void enableFXAA(bool enabled);

        std::shared_ptr<TechniqueCache> getTechniqueCache() { return m_techniqueCache; }
        std::shared_ptr<TextureManager> getTextureManager() { return m_textureManager; }
        std::shared_ptr<DeviceBufferManager> getDeviceBufferManager() { return m_deviceBufferManager; }
        std::shared_ptr<VertexAssembly> getVertexAssembly() { return m_vertexAssembly; }
        std::shared_ptr<FramebufferManager> getFramebufferManager() { return m_framebufferManager; }

    private:
        void enableDebugLogging();
        void renderPriv(std::vector<DrawItem>& drawItems);

        float m_aspectRatio;
        float m_timePassed;

        bool m_FXAAenabled;
        bool m_showGui;

        std::shared_ptr<Window> m_window;

        std::shared_ptr<GLContext> m_glContext;
        
        std::shared_ptr<TechniqueCache> m_techniqueCache;
        std::shared_ptr<TextureManager> m_textureManager;
        std::shared_ptr<DeviceBufferManager> m_deviceBufferManager;
        std::shared_ptr<VertexAssembly> m_vertexAssembly;
        std::shared_ptr<FramebufferManager> m_framebufferManager;

        std::shared_ptr<CameraController> m_cameraController;

        std::list<std::unique_ptr<RenderPass>> m_renderPasses;

        Scene* m_scene;
        std::shared_ptr<UIRenderer> m_uiRenderer;
    };
}