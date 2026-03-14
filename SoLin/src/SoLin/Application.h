#pragma once

#include"Core.h"
#include"Window.h"

#include"LayerStack.h"
#include"SoLin/ImGui/ImGuiLayer.h"
#include "SoLin/Events/ApplicationEvent.h"

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

		inline Window& GetWindow() { return *m_Window; }				//返回该应用窗口
		inline static Application& Get() { return *s_Instance; }		//返回该App单例指针
	private:
		bool OnWindowClose(WindowCloseEvent& event);

		ImGuiLayer* m_ImGuiLayer;
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	private:
		static Application* s_Instance;
	};

	// 这个将会在用户端被定义 
	// To be defined in client
	Application* CreateApplication();

}

