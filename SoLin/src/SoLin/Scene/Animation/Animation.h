#pragma once

#include"SoLin/Renderer/SubTexture2D.h"

namespace SoLin {

    struct AnimationFrame {
        // 帧图像
        Ref<SubTexture2D> image;
        // 帧时长
        float duration;
    };

    class Animation {
    public:
        Animation(const std::string& name = "default", bool loop = true);
        ~Animation() = default;

        // @brief 添加帧
        void AddFrame(const Ref<SubTexture2D> image, float duration);
        // @brief 获取给定时间点的帧
        const AnimationFrame& getFrame(float time) const;

        const std::string& getName() const { return m_Name; }
        void SetName(const std::string& name) { m_Name = name; }
        float getTotalDuration() const { return m_TotalDuration; }

        bool IsLooping()const { return m_Loop; }
        bool IsEmpty() const { return m_frames.empty(); }

        // @brief 获取帧数量
        size_t getFrameCount() const { return m_frames.size(); }
    private:
        // 动画名
        std::string m_Name;
        // 帧序列
        std::vector<AnimationFrame> m_frames;
        // 动画时长
        float m_TotalDuration = 0.0f;
        // 循环标志
        bool m_Loop = true;
    };
}
