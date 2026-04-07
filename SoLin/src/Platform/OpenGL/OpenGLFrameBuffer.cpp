#include "slpch.h"
#include "OpenGLFrameBuffer.h"

#include<glad/glad.h>

namespace SoLin {
    OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& spec)
        :m_Specification(spec)
    {
        ReCreate();
    }
    OpenGLFrameBuffer::~OpenGLFrameBuffer()
    {
        glDeleteFramebuffers(1, &m_RendererID);
        glDeleteTextures(1, &m_ColorAttachment);
        glDeleteRenderbuffers(1, &m_BufferAttachment);
    }
    void OpenGLFrameBuffer::ReCreate()
    {
        // 如果ID不为0，说明已经创建过，先删除再创建
        if (m_RendererID) {
            glDeleteFramebuffers(1, &m_RendererID);
            glDeleteTextures(1, &m_ColorAttachment);
            glDeleteRenderbuffers(1, &m_BufferAttachment);
        }

        // --------- 创建 帧缓冲区 对象 ---------------
        glCreateFramebuffers(1, &m_RendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        // --------- 创建 纹理 附件 ---------------
        glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
        glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        // 用于将 2D 纹理附加到 帧缓冲区对象 作为其附件（颜色、深度或模板缓冲区）。
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);
        // --------- 创建 渲染 缓冲区 对象 --------------
        glCreateRenderbuffers(1, &m_BufferAttachment);
        glBindRenderbuffer(GL_RENDERBUFFER, m_BufferAttachment);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_BufferAttachment);
        // --------- 检查 帧缓冲区 正确性 -----------
        SL_CORE_ASSERT((glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE), "Framebuffer is not complete");
        // --------- 解绑 帧缓冲区 ----------------------
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    void OpenGLFrameBuffer::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        // 及时为渲染结果更新视口
        glViewport(0, 0, m_Specification.Width, m_Specification.Height);
    }
    void OpenGLFrameBuffer::UnBind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    void OpenGLFrameBuffer::ReSize(uint32_t width, uint32_t height)
    {
        m_Specification.Width = width;
        m_Specification.Height = height;
    }
}
