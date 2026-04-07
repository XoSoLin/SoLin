#pragma once

#include"SoLin/Core/Core.h"
#include"SoLin/Core/Window.h"

#include"SoLin/Core/LayerStack.h"
#include"SoLin/ImGui/ImGuiLayer.h"
#include "SoLin/Events/ApplicationEvent.h"

#include"SoLin/Core/Timestep.h"

int main(int argc, char** argv);

namespace SoLin {
	
	class SOLIN_API Application
	{
	public:
		Application(const std::string& name = "SoLin App");
		virtual ~Application();

		void OnEvent(Event& e);				//事件分发

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		//@brief 返回该应用窗口
		inline Window& GetWindow() { return *m_Window; }
		//@brief 返回该App单例指针
		inline static Application& Get() { return *s_Instance; }
        //@brief 不同于 OnWindowClose ，WindowClose 是公有的关闭窗口函数，而 OnWindowClose 是回调函数中的事件处理
        inline void WindowClose() { m_Running = false; }
	private:
		void Run();                         // Run 函数现在为私有(Run 函数中定义 RunLoop)

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
        friend int ::main(int argc, char** argv);       //通过将 main 声明为友元函数，便可以在外部通过 main 来访问私有的 Run 函数
	};

	// 这个将会在用户端被定义 
	// To be defined in client
	Application* CreateApplication();

}

