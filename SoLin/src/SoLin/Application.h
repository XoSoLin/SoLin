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

		//@brief 返回该应用窗口
		inline Window& GetWindow() { return *m_Window; }
		//@brief 返回该App单例指针
		inline static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& event);

		bool m_Running = true;
		ImGuiLayer* m_ImGuiLayer;
		std::unique_ptr<Window> m_Window;

		LayerStack m_LayerStack;
	private:
		static Application* s_Instance;
	};

	// 这个将会在用户端被定义 
	// To be defined in client
	Application* CreateApplication();

}

