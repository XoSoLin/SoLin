#pragma once

#include"Core.h"
#include"Window.h"

#include "SoLin/Events/ApplicationEvent.h"
#include "SoLin/Events/MouseEvent.h"

namespace SoLin {
	
	class SOLIN_API Application
	{
	public:
		Application();
		virtual ~Application();

		void OnEvent(Event& e);				//事件分发
		void Run();
	private:
		bool OnWindowClose(WindowCloseEvent& event);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	// 这个将会在用户端被定义 
	// To be defined in client
	Application* CreateApplication();

}

