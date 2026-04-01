#pragma once

#include"Core.h"
#include"Window.h"

#include"LayerStack.h"
#include"SoLin/ImGui/ImGuiLayer.h"
#include "SoLin/Events/ApplicationEvent.h"

#include"SoLin/Core/Timestep.h"

namespace SoLin {
	
	class SOLIN_API Application
	{
	public:
		Application();
		virtual ~Application();

		void OnEvent(Event& e);				//事件分发
		void Run();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		//@brief 返回该应用窗口
		inline Window& GetWindow() { return *m_Window; }
		//@brief 返回该App单例指针
		inline static Application& Get() { return *s_Instance; }
	private:
		//@brief 窗口关闭回调
		bool OnWindowClose(WindowCloseEvent& event);
		//@brief 窗口尺寸回调
		bool OnWindowResize(WindowResizeEvent& event);

	private:
		bool m_Running = true;							//程序运行标志
		bool m_Minimized = false;
		float m_LastFrameTime = 0.0f;					//上一帧时间
		ImGuiLayer* m_ImGuiLayer;						//imgui层
		std::unique_ptr<Window> m_Window;				//程序的主窗口

		LayerStack m_LayerStack;						//程序的层栈
	private:
		static Application* s_Instance;
	};

	// 这个将会在用户端被定义 
	// To be defined in client
	Application* CreateApplication();

}

