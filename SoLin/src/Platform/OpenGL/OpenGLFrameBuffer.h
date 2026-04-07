#pragma once

#include "SoLin/Renderer/FrameBuffer.h"

namespace SoLin {

    // @brief OpenGL帧缓冲区
    class OpenGLFrameBuffer :public FrameBuffer {
    public:
        OpenGLFrameBuffer(const FrameBufferSpecification& spec);

        virtual ~OpenGLFrameBuffer();

        void ReSize();

        virtual void Bind() override;
        virtual void UnBind() override;

        virtual uint32_t GetColorAttachmentRendererID()const override { return m_ColorAttachment; }
        virtual const FrameBufferSpecification& GetSpecification()const override { return m_Specification; }

    private:
        uint32_t m_RendererID;                      // 该帧缓冲区类的ID

        uint32_t m_ColorAttachment;                 // 颜色附件的纹理ID
        uint32_t m_BufferAttachment;                // 附件的渲染缓冲区ID
        FrameBufferSpecification m_Specification;   // 帧缓冲区规范
    };
}
