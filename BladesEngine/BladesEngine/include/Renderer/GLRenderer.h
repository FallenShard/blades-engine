#pragma once

#include <vector>
#include <map>
#include <list>

#include "OpenGL.h"

#include "Window/Window.h"

#include "Renderer/TechniqueCache.h"
#include "Renderer/TextureManager.h"
#include "Renderer/FramebufferManager.h"
#include "Renderer/VertexAssembly.h"
#include "Renderer/DeviceBufferManager.h"

#include "Renderer/DrawItem.h"

#include "Camera/CameraController.h"

namespace fsi
{
    struct Event;
    class GLContext;
    class UIRenderer;
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

        void submitDrawItem(const DrawItem& drawItem);

        void setTessellationPatchVertices(int numberOfVertices);

        glm::ivec2 getScreenSize() const;

        std::shared_ptr<TechniqueCache> getTechniqueCache() { return m_techniqueCache; }
        std::shared_ptr<TextureManager> getTextureManager() { return m_textureManager; }
        std::shared_ptr<DeviceBufferManager> getDeviceBufferManager() { return m_deviceBufferManager; }
        std::shared_ptr<VertexAssembly> getVertexAssembly() { return m_vertexAssembly; }
        std::shared_ptr<FramebufferManager> getFramebufferManager() { return m_framebufferManager; }

    private:
        void enableDebugLogging();
        void renderScene();

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

        std::list<DrawItem> m_drawItems;

        std::shared_ptr<UIRenderer> m_uiRenderer;
    };
}