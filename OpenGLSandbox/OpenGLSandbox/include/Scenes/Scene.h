#pragma once

#include <memory>

#include "OpenGL.h"
#include "Window/Event.h"

#include "Renderer/ShaderProgram.h"
#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/VertexAttribute.h"
#include "Utils/VertexLoader.h"

class Scene
{
public:
    virtual void prepare() = 0;

    virtual void handleEvents(const Event& event) = 0;
    virtual void update(float timeDelta) = 0;
    virtual void render() = 0;

    virtual bool reshape(int width, int height) = 0;

protected:
    typedef std::unique_ptr<ShaderProgram> ProgramPtr;
    typedef std::unique_ptr<VertexArray> VertexArrayPtr;
    typedef std::unique_ptr<VertexBuffer> VertexBufferPtr;
    typedef std::unique_ptr<IndexBuffer> IndexBufferPtr;

    std::map<std::string, ProgramPtr> m_shaderPrograms;
    std::map<std::string, VertexArrayPtr> m_vertexArrays;
    std::map<std::string, VertexBufferPtr> m_vertexBuffers;
    std::map<std::string, IndexBufferPtr> m_indexBuffers;
};