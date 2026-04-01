#pragma once
#include "slpch.h"

#include "Core.h"
#include "SoLin/Events/Event.h"

namespace SoLin {
	// 窗口属性 windows properties
	struct WindowProps {
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "SoLin Engine",
			unsigned int width = 1280,
			unsigned int height = 720)
			:Title(title), Width(width), Height(height) {}
	};

	// 基于Windows的桌面系统的接口
	class SOLIN_API Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		//---------------------All interfaces------------------------------
		static Window* Create(const WindowProps& props = WindowProps());

		virtual ~Window(){}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		//Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;							//Vertical Sync(垂直同步)
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;
	};

}