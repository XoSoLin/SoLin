#pragma once

#include "SoLin/Renderer/FrameBuffer.h"

namespace SoLin {

    // @brief OpenGL帧缓冲区
    class OpenGLFrameBuffer :public FrameBuffer {
    public:
        OpenGLFrameBuffer(const FrameBufferSpecification& spec);

        virtual ~OpenGLFrameBuffer();

        //@brief 重创建
        void ReCreate();

        virtual void Bind() override;
        virtual void UnBind() override;

        virtual void ReSize(uint32_t width,uint32_t height) override;
        virtual uint32_t GetColorAttachmentRendererID()const override { return m_ColorAttachment; }
        virtual const FrameBufferSpecification& GetSpecification()const override { return m_Specification; }

    private:
        uint32_t m_RendererID = 0;                          // 该帧缓冲区类的ID

        uint32_t m_ColorAttachment = 0;                     // 颜色附件的纹理ID
        uint32_t m_BufferAttachment = 0;                    // 附件的渲染缓冲区ID
        FrameBufferSpecification m_Specification;           // 帧缓冲区规范
    };
}
