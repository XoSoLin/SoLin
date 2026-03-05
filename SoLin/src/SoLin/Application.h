#pragma once

#include"Core.h"

namespace SoLin {
	
	class SOLIN_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// 这个将会在用户端被定义 
	// To be defined in client
	Application* CreateApplication();

}

