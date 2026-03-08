#pragma once

#include"Core.h"
#include"Window.h"

namespace SoLin {
	
	class SOLIN_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	// 这个将会在用户端被定义 
	// To be defined in client
	Application* CreateApplication();

}

