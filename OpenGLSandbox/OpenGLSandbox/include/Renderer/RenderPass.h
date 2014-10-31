#pragma once

#include "Renderer/FrameBuffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/ShaderProgram.h"
#include "Renderer/Texture.h"

class RenderPass
{
public:
    RenderPass();
    ~RenderPass();

    void attachProgram(ShaderProgram* program);
    
    void init(int width, int height);
    void activate();

    void render();

private:
    FrameBuffer*   m_frameBuffer;
    Texture*       m_fullTex;
    VertexArray*   m_quadVertexArray;
    ShaderProgram* m_shaderProgram;

    GLuint         m_depthRenderBuffer;

    //VertexBuffer m_

};