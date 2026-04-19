#pragma once

namespace SoLin {

    // @breif 缓冲区 附件格式
    enum class FrameBufferAttachmentFormat {
        None = 0,
        // @brief 颜色 附件
        RGBA8,
        // @brief 红色通道整数 附件 (用于鼠标拾取实体ID)
        RED_INTEGER,
        // @brief 24位深度 8位模板 附件
        DEPTH24STENCIL8,
        // @brief 默认
        Depth = DEPTH24STENCIL8
    };

    // @brief 缓冲区附件规范 容器
    struct FrameBufferAttachmentsSpecification {
        FrameBufferAttachmentsSpecification() = default;
        FrameBufferAttachmentsSpecification(std::initializer_list<FrameBufferAttachmentFormat> attachments)
            :Attachments(attachments){ }

        std::vector<FrameBufferAttachmentFormat> Attachments;
    };

    // @brief 帧缓冲区必要数据
    struct FrameBufferSpecification
    {
        uint32_t Width;             // 宽像素
        uint32_t Height;            // 高像素
        uint32_t Samples = 1;       // 采样次数

        FrameBufferAttachmentsSpecification Attachments;// 帧缓冲区各个附件的规范
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
        // @brief 从帧缓冲区的特定附件中读取单个像素的整数值
        // (比如该整数值代表具体的实体ID)
        // @param attachmentIndex   附件索引
        // @param x,y   要读取的像素坐标
        virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;
        // @brief 清理对应附件
        // @param attachmentIndex 附件索引
        // @param value 覆盖值
        virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;

        // @brief 获取附加色彩的渲染ID
        // @param Index 颜色附件索引
        virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0)const = 0;
        // @brief 获取帧缓冲区必要数据
        virtual const FrameBufferSpecification& GetSpecification() const = 0;
    };

}

