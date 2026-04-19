#include "slpch.h"
#include "OpenGLFrameBuffer.h"

#include<glad/glad.h>

// -------------------------------------------------------------------------------------------
// ------------------------------------ Utils ------------------------------------------------
// -------------------------------------------------------------------------------------------
namespace SoLin::Utils
{
    // @brief 获取 纹理模式
    // @return 多重采样的2d纹理 或 普通纹理
    static GLenum TextureTarget(bool multisampled)
    {
        return multisampled == true ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
    }

    // @brief 判断是否为深度格式
    static bool IsDepthFormat(SoLin::FrameBufferAttachmentFormat format)
    {
        switch (format)
        {
        case SoLin::FrameBufferAttachmentFormat::DEPTH24STENCIL8: return true;
        }
        return false;
    }

    // @brief SoLin附件内部格式转换为OpenGL
    static GLenum SoLinTexFormatToGL(SoLin::FrameBufferAttachmentFormat format){
        switch (format) {
        case SoLin::FrameBufferAttachmentFormat::RGBA8:
            return GL_RGBA8;
        case SoLin::FrameBufferAttachmentFormat::RED_INTEGER:
            return GL_RED_INTEGER;
        }
    }

    // @brief 创建纹理接口
    // @param multisampled 多重采样标志
    // @param count 创建个数
    // @param id id数据地址
    static void CreateTextures(bool multisampled, uint32_t count, uint32_t* id)
    {
        glCreateTextures(TextureTarget(multisampled), count, id);
    }

    static void BindTexture(bool multisampled, uint32_t id)
    {
        glBindTexture(TextureTarget(multisampled), id);
    }

    // @brief 附加颜色纹理
    // @param id                纹理对象ID
    // @param samples           采样次数
    // @param internalFormat    内部存储格式(GPU)
    // @param format            像素排列格式(CPU)
    // @param width             纹理宽度
    // @param height            纹理高度
    // @param index             颜色附着点索引
    static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat,GLenum format, uint32_t width, uint32_t height, int index)
    {
        bool multisampled = samples > 1;
        // 多重采样
        if (multisampled)
        {
            // 创建一个多重采样纹理（每个像素包含 samples 个采样点）
            // 多重采样纹理不支持传统的纹理过滤参数（glTexParameteri 的 GL_LINEAR 等），所以没有后续的参数设置。
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
        }
        // 普通纹理
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);       // 缩小过滤器：线性插值
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);       // 放大过滤器：线性插值
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);    // R方向（3D纹理用）
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);    // S方向（X轴）边缘截断
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);    // T方向（Y轴）边缘截断
        }

        // 将纹理附加到当前绑定的帧缓冲对象的某个附着点
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
    }

    // @brief 附加附件纹理
    // @param id                纹理ID
    // @param samples           采样次数
    // @param format            内部格式
    // @param attachmentType    附件类型
    // @param width             纹理宽度
    // @param height            纹理高度
    static void AttachBufferTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
    {
        bool multisampled = samples > 1;
        // 多重采样
        if (multisampled)
        {
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
        }
        // 普通纹理
        else
        {
            glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);      // 缩小过滤器：线性插值
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);      // 放大过滤器：线性插值
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);   // R方向（3D纹理用）
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);   // S方向（X轴）边缘截断
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);   // T方向（Y轴）边缘截断

            // 也可以使用渲染缓冲区对象来附加深度/模板附件
            //glCreateRenderbuffers(1, &id);
            //glBindRenderbuffer(GL_RENDERBUFFER, id);
            //glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
            //glBindRenderbuffer(GL_RENDERBUFFER, 0);
        }
        //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_BufferAttachment);
        
        // 用于将 2D 纹理附加到 帧缓冲区对象 作为其附件（深度或模板缓冲区）。
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
    }
}

// -------------------------------------------------------------------------------------------
// -------------------------------------- SoLin ----------------------------------------------
// -------------------------------------------------------------------------------------------

namespace SoLin {
    OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& spec)
        :m_Specification(spec)
    {
        for (auto spec : m_Specification.Attachments.Attachments)
        {
            if (!Utils::IsDepthFormat(spec))
                m_ColorAttachmentSpecifications.emplace_back(spec);
            else
                m_BufferAttachmentSpecification = spec;
        }
        ReCreate();
    }
    OpenGLFrameBuffer::~OpenGLFrameBuffer()
    {
        glDeleteFramebuffers(1, &m_RendererID);
        glDeleteTextures(m_ColorAttachmentIDs.size(), m_ColorAttachmentIDs.data());
        glDeleteRenderbuffers(1, &m_BufferAttachmentID);
    }
    void OpenGLFrameBuffer::ReCreate()
    {
        // 如果ID不为0，说明已经创建过，先删除再创建
        if (m_RendererID) {
            glDeleteFramebuffers(1, &m_RendererID);
            glDeleteTextures(m_ColorAttachmentIDs.size(), m_ColorAttachmentIDs.data());
            glDeleteRenderbuffers(1, &m_BufferAttachmentID);

            m_ColorAttachmentIDs.clear();
            m_BufferAttachmentID = 0;
        }

        // --------- 创建 帧缓冲区 对象 ---------------
        glCreateFramebuffers(1, &m_RendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

        // --------- 创建 纹理 附件 ---------------

        // 多重采样标志（帧缓冲区数据样品数大于1时启用）
        // 多重采样（MSAA）：
        // 同一个像素内部，有多个采样点，用来抗锯齿。
        bool multisample = m_Specification.Samples > 1;
        if (m_ColorAttachmentSpecifications.size())
        {
            m_ColorAttachmentIDs.resize(m_ColorAttachmentSpecifications.size());
            // 批量创建纹理
            Utils::CreateTextures(multisample, m_ColorAttachmentIDs.size(), m_ColorAttachmentIDs.data());
            // 逐个配置纹理
            for (size_t i = 0; i < m_ColorAttachmentIDs.size(); i++)
            {
                Utils::BindTexture(multisample, m_ColorAttachmentIDs[i]);
                switch (m_ColorAttachmentSpecifications[i])
                {
                case FrameBufferAttachmentFormat::RGBA8:
                    Utils::AttachColorTexture(m_ColorAttachmentIDs[i], m_Specification.Samples, GL_RGBA8,GL_RGBA, m_Specification.Width, m_Specification.Height, i);
                    break;
                case FrameBufferAttachmentFormat::RED_INTEGER:
                    Utils::AttachColorTexture(m_ColorAttachmentIDs[i], m_Specification.Samples, GL_R32I, GL_RED_INTEGER, m_Specification.Width, m_Specification.Height, i);
                    break;
                }
            }
        }

        // --------- 创建 深度/模板 附件 --------------
        if (m_BufferAttachmentSpecification != FrameBufferAttachmentFormat::None)
        {
            Utils::CreateTextures(multisample, 1, &m_BufferAttachmentID);
            Utils::BindTexture(multisample, m_BufferAttachmentID);
            switch (m_BufferAttachmentSpecification)
            {
            case FrameBufferAttachmentFormat::DEPTH24STENCIL8:
                Utils::AttachBufferTexture(m_BufferAttachmentID, m_Specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.Width, m_Specification.Height);
                break;
            }
        }

        // 将片段着色器的输出变量映射到多个不同的颜色附件(MRT)
        // 多个颜色附件（MRT）：
        // 同一个像素，输出多份不同的数据（比如颜色、法线、位置），用来做复杂渲染（如延迟渲染）。
        if (m_ColorAttachmentIDs.size() > 1)
        {
            SL_CORE_ASSERT((m_ColorAttachmentIDs.size() <= 4), "");

            GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
            // 这个函数告诉 OpenGL：片段着色器的多个输出应该写入哪些颜色附件
            glDrawBuffers(m_ColorAttachmentIDs.size(), buffers);
        }
        // 无颜色附件情况
        else if (m_ColorAttachmentIDs.empty())
        {
            // Only depth pass
            glDrawBuffer(GL_NONE);
        }

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

        // 更新数据后要重新创建
        ReCreate();
    }
    int OpenGLFrameBuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
    {
        // 确保要读取的附件索引小于总数
        SL_CORE_ASSERT((attachmentIndex < m_ColorAttachmentIDs.size()), "Attachment index is beyond the scope of Attachments which we set");

        // 告诉 OpenGL 接下来从哪个颜色附件读取像素
        glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);

        // 从当前绑定的帧缓冲区读取一个像素
        // GL_RED_INTEGER 指红色通道
        int pixelData;
        glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);

        return pixelData;
    }

    void OpenGLFrameBuffer::ClearAttachment(uint32_t attachmentIndex, int value)
    {
        SL_CORE_ASSERT((attachmentIndex < m_ColorAttachmentIDs.size()), "Make sure that attachment_index you typed is in the scope of Attachments which we set");

        auto& spec = m_ColorAttachmentSpecifications[attachmentIndex];
        glClearTexImage(m_ColorAttachmentIDs[attachmentIndex], 0, Utils::SoLinTexFormatToGL(spec), GL_INT, &value);
    }

}
