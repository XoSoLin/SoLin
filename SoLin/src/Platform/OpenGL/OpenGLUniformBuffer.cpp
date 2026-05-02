#include"slpch.h"
#include"Platform/OpenGL/OpenGLUniformBuffer.h"

#include<glad/glad.h>

namespace SoLin {
    OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size, uint32_t binding)
    {
        glCreateBuffers(1, &m_RendererID);
        // 为指定的缓冲区对象分配并初始化数据存储
        glNamedBufferData(m_RendererID, size, nullptr, GL_DYNAMIC_DRAW);
        // 将缓冲区绑定到 Uniform Buffer Binding Point 上，
        // 然后着色器中通过 layout(binding = index) 的 uniform block 来获取数据。
        glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RendererID);
    }
    OpenGLUniformBuffer::~OpenGLUniformBuffer()
    {
        glDeleteBuffers(1, &m_RendererID);
    }
    void OpenGLUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
    {
        // 更新缓冲区部分数据
        glNamedBufferSubData(m_RendererID, offset, size, data);
    }
}
