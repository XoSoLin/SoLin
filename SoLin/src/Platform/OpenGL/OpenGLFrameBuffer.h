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
        virtual int ReadPixel(uint32_t attachmentIndex, int x, int y)override;
        virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0)const override
        {
            SL_CORE_ASSERT((index < m_ColorAttachmentIDs.size()),"Index should be less than the number of all Color attachments");
            return m_ColorAttachmentIDs[index];
        }
        virtual const FrameBufferSpecification& GetSpecification()const override { return m_Specification; }

    private:
        uint32_t m_RendererID = 0;                          // 该帧缓冲区类的ID

        FrameBufferSpecification m_Specification;           // 帧缓冲区规范数据

        std::vector<FrameBufferAttachmentFormat> m_ColorAttachmentSpecifications;// 颜色附件的规范列表
        FrameBufferAttachmentFormat m_BufferAttachmentSpecification;// 深度、模板 附件规范

        std::vector<uint32_t> m_ColorAttachmentIDs;         // 颜色附件的纹理ID列表
        uint32_t m_BufferAttachmentID = 0;                  // 附件的渲染缓冲区ID
    };
}
