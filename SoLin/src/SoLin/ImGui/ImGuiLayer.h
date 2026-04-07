#pragma once

#include"SoLin/Core/Layer.h"
#include "SoLin/Events/ApplicationEvent.h"
#include "SoLin/Events/KeyEvent.h"
#include "SoLin/Events/MouseEvent.h"

namespace SoLin {

	class SOLIN_API ImGuiLayer:public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach()override;
		virtual void OnDetach()override;
        virtual void OnEvent(Event& e) override;
		virtual void OnImGuiRender()override;

		void Begin();
		void End();
        // @brief 设置阻塞事件标志
        void BlockEvents(bool block) { m_BlockEvents = block; }
	private:
		float m_Time = 0.0f;
        bool m_BlockEvents = true;  // 接收(阻塞)事件标志
	};
}

