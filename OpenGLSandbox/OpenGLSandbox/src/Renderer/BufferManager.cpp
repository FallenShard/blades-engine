#include "Renderer/BufferManager.h"



namespace fsi
{

namespace
{
    static std::map<VertexAttrib::ID, VertexBufferBinding::ID> initBindingTable()
    {
        std::map<VertexAttrib::ID, VertexBufferBinding::ID> table;

        table[VertexAttrib::Position] = VertexBufferBinding::Position;
        table[VertexAttrib::Normal] = VertexBufferBinding::Normal;
        table[VertexAttrib::TexCoord0] = VertexBufferBinding::TexCoord0;

        return table;
    }
}

std::map<VertexAttrib::ID, VertexBufferBinding::ID> BufferManager::bindingTable = initBindingTable();

BufferManager::BufferManager()
{

}

BufferManager::~BufferManager()
{

}

RenderPackage BufferManager::submitMesh(Mesh* mesh)
{
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    std::shared_ptr<VertexBuffer> vbo = std::make_shared<VertexBuffer>(GL_STATIC_DRAW);
    vbo->bind();
    vbo->create(*mesh->getVertexData().get());

    const std::set<VertexFormat>& attribFormats = mesh->getVertexFormats();

    for (auto& attrib : attribFormats)
    {
        glEnableVertexAttribArray(attrib.index);
        //vao->enableVertexAttrib(attrib.index);
        glVertexAttribFormat(attrib.index, attrib.size, attrib.type, attrib.normalized, attrib.offset);
        //vao->attributeFormat(attrib);
        glVertexAttribBinding(attrib.index, bindingTable[attrib.index]);
        //vao->attributeBinding(attrib.index, bindingTable[attrib.index]);
        
        vbo->bind(bindingTable[attrib.index], attrib.offset, mesh->getVertexSize() * 4);
    }

    std::shared_ptr<IndexBuffer> ibo = std::make_shared<IndexBuffer>();

    ibo->bind();
    ibo->create(*mesh->getIndices().get());

    //vao->setVertexCount(mesh->getVertexCount());
    //vao->setIndexCount(mesh->getIndexCount());

    VertexArray::release();

    RenderPackage package;
    package.vertexArrayId = vao;
    package.indexCount = mesh->getIndexCount();
    package.vertexCount = mesh->getVertexCount();
    return package;
}

}