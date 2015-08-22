#pragma once

#include "PostProcessing/RenderPass.h"
#include "Renderer/GLRenderer.h"

namespace fsi
{
    class Technique;

    class FXAA : public RenderPass
    {
    public:
        FXAA(int width, int height, GLRenderer* technique);
        ~FXAA();

        virtual void setAsSurface();
        virtual void render();
        virtual void resize(int width, int height);

    private:
        void init(int width, int height);
        void initFBO(int width, int height);
        void initVAO();

        void cleanUpFBO();

        GLuint m_vao;

        GLuint m_fbo;
        GLuint m_tex;
        GLuint m_sampler;
        GLuint m_rbo;

        GLRenderer* m_renderer;
        std::shared_ptr<Technique> m_technique;
    };
}