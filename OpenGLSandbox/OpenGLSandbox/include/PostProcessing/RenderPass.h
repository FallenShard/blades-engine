#pragma once

#include "OglWrapper/FrameBuffer.h"
#include "OglWrapper/VertexArray.h"
#include "OglWrapper/VertexBuffer.h"
#include "OglWrapper/ShaderProgram.h"
#include "OglWrapper/Texture.h"

namespace fsi
{

class RenderPass
{
public:
    RenderPass();
    ~RenderPass();

    void attachProgram(ShaderProgram* program);
    
    void init(int width, int height);
    void activate();

    void resize(int width, int height);

    void render();

private:
    void initFBO(int width, int height);
    void initVAO();

    void cleanUpFBO();

    GLuint m_vao;

    GLuint m_fbo;
    GLuint m_tex;
    GLuint m_sampler;
    GLuint m_rbo;

    //FrameBuffer*   m_frameBuffer;
    //Texture*       m_fullTex;
    //VertexArray*   m_quadVertexArray;
    ShaderProgram* m_shaderProgram;

    

    //VertexBuffer m_

};

}