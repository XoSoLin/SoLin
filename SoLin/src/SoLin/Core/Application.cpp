#include "slpch.h"
#include<GLFW/glfw3.h>
#include "SoLin/Core/Application.h"

#include "SoLin/Events/Event.h"
#include "SoLin/Core/Log.h"
#include"Input.h"

#include"SoLin/Renderer/Renderer.h"

namespace SoLin {

#define BIND_EVENT_FN(x) std::bind(&Application::x,this,std::placeholders::_1)		//绑定一个成员函数，并占位参数
																					//std::bind()用于将一个可调度对象与一些参数绑定在一起，生成一个新的可调度对象,
																					//	其返回的对象有了额外的上下文，不是一个单纯的指针
																					//this 指的就是当前这个对象，是this对象的成员Application::x()的第一个隐藏参数
																					//std::placeholders::_1是传入这个新的可调度对象的一个参数，这里是指 Event&
																					// 以void(int& a,int& b)为例,其为f1   
																					// 当f2 = bind(f1,this,占位符2,占位符1)之后，f1(1,2)就相当于f2(2,1)

	Application* Application::s_Instance = nullptr;									//初始化唯一实例的静态成员s_Instance;

	
	Application::Application()
	{
        SL_PROFILE_FUNCTION();

		// 如果Appliction再次被构造，将会触发ASSERT，因为第一次构造后将会使s_Instance不再为nullptr
		SL_CORE_ASSERT(!s_Instance, "Application already exists!(The class Application is a Singleton,it just support one instance!)");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());

		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
		m_Window->SetVSync(true);
		Renderer::Init();


		m_ImGuiLayer = new ImGuiLayer();												//初始化 m_ImGuiLayer 为原始指针，并推入层栈
		PushOverlay(m_ImGuiLayer);

	}

	Application::~Application()
	{
	}
	
	void Application::PushLayer(Layer* layer) {
        SL_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
        layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay) {
        SL_PROFILE_FUNCTION();

		m_LayerStack.PushOverLay(overlay);
        overlay->OnAttach();
	}

	void Application::OnEvent(Event& e) {
        SL_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

		// 这里会输出所有e的信息，但这些事件并不是都被响应处理了。
		// 目前只有WindowCloseEvent被处理，也就是执行了OnWindowClose()
		//SL_CORE_TRACE("{0} | handled({1})", e.ToString(),e.Handled);

		for (auto iter = m_LayerStack.end(); iter != m_LayerStack.begin();) {
			(*--iter)->OnEvent(e);
			if (e.Handled) {
				break;
			}
		}

	}

	void Application::Run() {
        SL_PROFILE_FUNCTION();

		WindowResizeEvent e(1280, 720);
		
		while (m_Running) {
            SL_PROFILE_SCOPE("RunLoop");

			float time = (float)glfwGetTime();				//获取当前时间
			Timestep timestep = time - m_LastFrameTime;		//计算变化时间
			m_LastFrameTime = time;							//存储当前时间供下次使用
			
			if (!m_Minimized) {
                {
                    SL_PROFILE_SCOPE("LayerStack OnUpdate");

				    for (Layer* layer : m_LayerStack) {				//更新图层
					    layer->OnUpdate(timestep);
				    }
                }
			}


			m_ImGuiLayer->Begin();

            {
                SL_PROFILE_SCOPE("LayerStack OnImGuiRenderer");

                for (Layer* layer : m_LayerStack)
                    layer->OnImGuiRender();
            }
            m_ImGuiLayer->End();

			m_Window->OnUpdate();							//更新窗口
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& event) {
		m_Running = false;
		return true;
	}
	bool Application::OnWindowResize(WindowResizeEvent& event)
	{
        SL_PROFILE_FUNCTION();

		m_Minimized = false;
		Renderer::OnWindowResize(event.GetWidth(), event.GetHeight());
		
		if (event.GetWidth() == 0 && event.GetHeight() == 0) {
			m_Minimized = true;
			return false;
		}

		return false;
	}
}
