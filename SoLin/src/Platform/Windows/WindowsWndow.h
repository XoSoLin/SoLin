#pragma once

#include "SoLin/Core/Window.h"

#include"SoLin/Renderer/GraphicsContext.h"
#include<GLFW/glfw3.h>

namespace SoLin {

    // @brief window平台下的窗口类
    // @brief 该类的实现目前还依赖OpenGL，后续更改
	class WindowsWindow:public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline uint32_t GetWidth() const override { return m_Data.Width; }
		inline uint32_t GetHeight() const override { return m_Data.Height; }

		inline void SetEventCallback(const EventCallbackFn& callback)override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled)override;
		bool IsVSync()const override;

		inline void* GetNativeWindow()const override { return m_Window; }

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		std::unique_ptr<GraphicsContext> m_Context;

		struct WindowData {
			std::string Title;
            uint32_t Width;
            uint32_t Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};

}

