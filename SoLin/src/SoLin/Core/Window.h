#pragma once
#include "slpch.h"

#include "Core.h"
#include "SoLin/Events/Event.h"

namespace SoLin {
	// 窗口属性 windows properties
	struct WindowProps {
		std::string Title;
        uint32_t Width;
        uint32_t Height;

		WindowProps(const std::string& title = "SoLin Engine",
            uint32_t width = 1280,
            uint32_t height = 720)
			:Title(title), Width(width), Height(height) {}
	};

	// 基于Windows的桌面系统的接口
	class SOLIN_API Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		//---------------------All interfaces------------------------------
		static std::unique_ptr<Window> Create(const WindowProps& props = WindowProps());

		virtual ~Window(){}

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		//Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;							//Vertical Sync(垂直同步)
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;
	};

}
