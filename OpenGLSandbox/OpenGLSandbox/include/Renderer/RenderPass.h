#pragma once

#include "OglWrapper/FrameBuffer.h"
#include "OglWrapper/VertexArray.h"
#include "OglWrapper/VertexBuffer.h"
#include "OglWrapper/ShaderProgram.h"
#include "OglWrapper/Texture.h"

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