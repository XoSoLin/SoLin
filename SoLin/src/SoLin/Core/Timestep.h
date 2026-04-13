#pragma once

#include<chrono>
#include<thread>
namespace SoLin {

	class Timestep {
	public:
		Timestep(float fps)
			:m_TargetFPS(fps)
		{
            Reset();
        }
        // @brief 更新时间
        void Tick();
        // @brief 重置时间
        void Reset();

        //void operator() { Tick(); }
        // @brief 重载了float类型转换
		operator float() const { return m_DeltaTime; }			//使其方便参与计算

		//@brief 获取秒
		float GetSeconds()const { return m_DeltaTime; }
		//@brief 获取毫秒
		float GetMilliseconds()const { return m_DeltaTime * 1000.0f; }

    public:
        void SetTargetFPS(float fps);
        float GetTargetFPS()const { return m_TargetFPS; }
	private:
        using Clock = std::chrono::steady_clock;                //单调递增的时钟，不受系统时间调整影响
        using TimePoint = Clock::time_point;                    //时间点，记录某个时刻
        using TimeDuration = Clock::duration;      //时间段，表示两个时间点的间隔

        TimePoint m_LastPoint;          // 上一帧时间
		float m_DeltaTime;              // 帧间隔时间       
        float m_TotalTime;              // 累计运行时间
        bool m_IsFirstFrame;            // 第一帧标志
        float m_TimeScale;              // 时间缩放因子

    private:
        float m_TargetFPS;              // 目标帧率
        float m_TargetFrameTime;        // 目标每帧间隔
        TimeDuration m_TargetDuration;  // 目标时间间隔
        TimePoint m_NextFramePoint;
	};
}
