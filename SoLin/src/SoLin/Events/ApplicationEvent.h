#pragma once

#include"Event.h"

#include<sstream>

namespace SoLin {
	class SOLIN_API WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(unsigned int width,unsigned int height) 
			:m_Width(width),m_Height(height){}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

		std::string ToString() const override {
			std::stringstream ss;
			ss << "WindowResizeEvent: " << GetWidth() << "," << GetHeight();
			return ss.str();
		}

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }
	private:
		unsigned int m_Width, m_Height;
	};

	class SOLIN_API WindowCloseEvent :public Event {
	public:
		WindowCloseEvent(){}
		
		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class SOLIN_API AppTickEvent :public Event {
	public:
		AppTickEvent() {}

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class SOLIN_API AppUpdateEvent :public Event {
	public:
		AppUpdateEvent() {}

		EVENT_CLASS_TYPE(AppUpdate)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class SOLIN_API AppRenderEvent :public Event {
	public:
		AppRenderEvent() {}

		EVENT_CLASS_TYPE(AppRender)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	// TO ADD 
	// Application Event
}