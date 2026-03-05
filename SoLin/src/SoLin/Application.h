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

	// 将会在客户端定义 To be defined in client
	Application* CreateApplication();

}

