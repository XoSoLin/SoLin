#pragma once

namespace SoLin {

    // @brief 帧缓冲区必要数据
    struct FrameBufferSpecification
    {
        uint32_t Width;             // 宽像素
        uint32_t Height;            // 高像素
        uint32_t Samples = 1;
    };

    // @brief 帧缓冲区
    class FrameBuffer {
    public:
        // @brief 根据渲染API创建帧缓冲区
        static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);
        virtual ~FrameBuffer() = default;

        virtual void Bind() = 0;
        virtual void UnBind() = 0;

        // @brief 更新尺寸
        virtual void ReSize(uint32_t width, uint32_t height) = 0;

        // @brief 获取附加色彩的渲染ID
        virtual uint32_t GetColorAttachmentRendererID()const = 0;
        // @brief 获取帧缓冲区必要数据
        virtual const FrameBufferSpecification& GetSpecification() const = 0;
    };

}

