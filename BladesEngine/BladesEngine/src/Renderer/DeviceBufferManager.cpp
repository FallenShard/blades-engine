#include "Renderer/DeviceBufferManager.h"



namespace fsi
{
    DeviceBufferManager::DeviceBufferManager()
    {

    }

    DeviceBufferManager::~DeviceBufferManager()
    {
        for (auto& element : m_bufferUsageMap)
            glDeleteBuffers(1, &element.first);
        m_bufferUsageMap.clear();
    }


    GLuint DeviceBufferManager::allocate(GLsizei byteSize, GLenum flags)
    {
        GLuint buffer;
        glCreateBuffers(1, &buffer);

        glNamedBufferStorage(buffer, byteSize, nullptr, flags);

        m_bufferUsageMap.emplace(buffer, flags);

        return buffer;
    }

    GLuint DeviceBufferManager::allocateMutable(GLsizei byteSize, GLenum usageHint)
    {
        GLuint buffer;
        glCreateBuffers(1, &buffer);

        glNamedBufferData(buffer, byteSize, nullptr, usageHint);

        m_bufferUsageMap.emplace(buffer, usageHint);

        return buffer;
    }

    void DeviceBufferManager::deleteBuffer(GLuint buffer)
    {
        glDeleteBuffers(1, &buffer);
        m_bufferUsageMap.erase(buffer);
    }

    void DeviceBufferManager::update(GLuint buffer, const void* data, GLsizei size, GLint offset)
    {
        auto mappedBuffer = glMapNamedBufferRange(buffer, offset, size, m_bufferUsageMap[buffer]);
        memcpy(mappedBuffer, data, size);
        glUnmapNamedBuffer(buffer);
    }
}