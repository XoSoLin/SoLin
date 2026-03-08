#include "slpch.h"
#include "Application.h"

#include "Events/Event.h"
#include "SoLin/Events/ApplicationEvent.h"
#include "SoLin/Log.h"

namespace SoLin {

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application()
	{
	}

	void Application::Run() {
		WindowResizeEvent e(1280, 720);
		
		//SL_CLIENT_INFO(e);								//[error]	重载了<< 也在log.h加了#include"spdlog/fmt/ostr.h" 但是这个调用还是不行
															//6、7年前的教程是可行的，不知道为什么这里不行
		if (e.IsInCategory(EventCategoryApplication)) {
			SL_CLIENT_TRACE(e.ToString());
		}
		if (e.IsInCategory(EventCategoryMouseButton)) {
			SL_CLIENT_TRACE(e.ToString());
		}
		while (m_Running) {
			m_Window->OnUpdate();
		}
	}
}
