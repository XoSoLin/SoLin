#include"slpch.h"
#include"Animation.h"
#include<glm/common.hpp>

namespace SoLin {
    Animation::Animation(const std::string& name, bool loop)
        :m_Name(name),m_Loop(loop)
    {
    }

    void Animation::AddFrame(const Ref<SubTexture2D> image, float duration)
    {
        if (duration < 0.0f) {
            SL_CORE_ERROR("{0}:添加无效帧",m_Name);
            return;
        }
        SL_CORE_INFO("{0}:添加帧{1}", m_Name, getFrameCount());
        m_frames.push_back({ image,duration });
        m_TotalDuration += duration;
    }

    const AnimationFrame& Animation::getFrame(float time) const
    {
        // 空帧情况
        if (IsEmpty()) {
            SL_CORE_ERROR("{0}:该动画无帧", m_Name);
            return{nullptr,0.0f};
        }

        // 模运算 提取当前时间
        float current_time = time;
        if (m_Loop && m_TotalDuration > 0.0f) {
            current_time = glm::mod(time, m_TotalDuration);
        }
        else {
            if (current_time >= m_TotalDuration) {
                return m_frames.back();
            }
        }

        // 正常计算当前帧
        float accumulated_time = 0.0f;
        for (const auto& frame : m_frames) {
            accumulated_time += frame.duration;
            if (current_time < accumulated_time) {
                return frame;
            }
        }
    }
}
