#pragma once
namespace SoLin {

	class Timestep {
	public:
		Timestep(float time)
			:m_Time(time)
		{}
		operator float() const { return m_Time; }			//使其方便参与计算

		//@brief 获取秒
		float GetSeconds()const { return m_Time; }
		//@brief 获取毫秒
		float GetMilliseconds()const { return m_Time * 1000.0f; }
	private:
		float m_Time;
	};
}