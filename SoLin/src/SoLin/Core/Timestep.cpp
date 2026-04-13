#include "slpch.h"

#include"Timestep.h"

namespace SoLin {
    void Timestep::Tick()
    {
        // 获取当前时刻
        TimePoint currentPoint = Clock::now();

        // 首帧特殊处理
        if (m_IsFirstFrame) {
            m_LastPoint = currentPoint;
            m_IsFirstFrame = false;
            m_DeltaTime = 0.0f;
            return;
        }

        // 以float秒形式获取时间间隔
        TimeDuration duration = currentPoint - m_LastPoint;
        //auto currentDeltaTime = duration.count();
        // 过快限制帧率
        if (duration < m_TargetDuration) {
            // 等待到目标时间点
            std::this_thread::sleep_until(m_LastPoint + m_TargetDuration);
            // 浮点秒转换为浮点数传出
            m_DeltaTime = std::chrono::duration<float>(m_TargetDuration).count();
            m_TotalTime += m_DeltaTime;// 统计总时间
            m_LastPoint = m_LastPoint + m_TargetDuration;// 记录本帧开始时间，做下次循环时的上一帧时间
            
        }
        // 已超过目标间隔时间，不做限制
        else {
            // 转换为浮点秒传出
            m_DeltaTime = std::chrono::duration<float>(duration).count();
            m_TotalTime += m_DeltaTime;// 统计总时间
            m_LastPoint = currentPoint;// 记录本帧开始时间，做下次循环时的上一帧时间
        }
    }

    void Timestep::Reset()
    {
        m_LastPoint = Clock::now();
        m_DeltaTime = 0.0f;
        m_TotalTime = 0.0f;
        m_IsFirstFrame = true;
    }

    void Timestep::SetTargetFPS(float fps)
    {
        if (fps < 0) {
            m_TargetFPS = 0;
        }
        else {
            m_TargetFPS = fps;
        }
        if (fps > 0) {
            m_TargetFrameTime = 1.0f / m_TargetFPS;
            auto d = std::chrono::duration<float>(m_TargetFrameTime);
            m_TargetDuration = std::chrono::duration_cast<std::chrono::steady_clock::duration>(d);
        }
        else {
            m_TargetFrameTime = 0.0f;
        }
    }
}
