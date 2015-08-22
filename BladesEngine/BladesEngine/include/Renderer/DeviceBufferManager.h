#pragma once

#include <map>
#include <vector>
#include "OpenGL.h"

namespace fsi
{
    class DeviceBufferManager
    {
    public:
        DeviceBufferManager();
        ~DeviceBufferManager();

        DeviceBufferManager(const DeviceBufferManager& rhs) = delete;
        DeviceBufferManager(DeviceBufferManager&& rhs) = delete;

        GLuint allocate(GLsizei byteSize, GLenum usageHint);
        void deleteBuffer(GLuint buffer);

        template <typename T>
        void update(GLuint buffer, const std::vector<T>& data, GLint offset = 0)
        {
            T* mappedBuffer = reinterpret_cast<T*>(glMapNamedBufferRange(buffer, offset, data.size() * sizeof(T), m_bufferUsageMap[buffer]));
            memcpy(mappedBuffer, data.data(), data.size() * sizeof(T));
            glUnmapNamedBuffer(buffer);
        }

        void update(GLuint buffer, const void* data, GLsizei size, GLint offset = 0);

    private:
        std::map<GLuint, GLenum> m_bufferUsageMap;
    };
}