#pragma once

#include"SoLin/Layer.h"
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
		virtual void OnImGuiRender()override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}

