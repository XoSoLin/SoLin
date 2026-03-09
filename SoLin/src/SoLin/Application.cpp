#include "slpch.h"
#include "Application.h"

#include "Events/Event.h"
#include "SoLin/Log.h"

#include <GLFW/glfw3.h>

namespace SoLin {

#define BIND_EVENT_FN(x) std::bind(&Application::x,this,std::placeholders::_1)		//绑定一个成员函数，并占位参数
																					//std::bind()用于将一个可调度对象与一些参数绑定在一起，生成一个新的可调度对象,
																					//	其返回的对象有了额外的上下文，不是一个单纯的指针
																					//this 指的就是当前这个对象，是this对象的成员Application::x()的第一个隐藏参数
																					//std::placeholders::_1是传入这个新的可调度对象的一个参数，这里是指 Event&
																					// 以void(int& a,int& b)为例,其为f1   
																					// 当f2 = bind(f1,this,占位符2,占位符1)之后，f1(1,2)就相当于f2(2,1)

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());

		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
		/*auto f2 = std::bind(&Application::OnEvent,this,std::placeholders::_1);
		m_Window->SetEventCallback(f2);*/
	}

	Application::~Application()
	{
	}

	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		// 这里会输出所有e的信息，但这些事件并不是都被响应处理了。
		// 目前只有WindowCloseEvent被处理，也就是执行了OnWindowClose()
		SL_CORE_TRACE("{0}", e.ToString());
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
			glClearColor(0.8, 0.1, 0.9, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& event) {
		m_Running = false;
		return true;
	}
}
