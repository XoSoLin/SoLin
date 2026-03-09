#pragma once

#include "Event.h"

namespace SoLin {

	//@brief 鼠标移动事件
	class SOLIN_API MouseMovedEvent :public Event {
	public:
		MouseMovedEvent(float x,float y)
			:m_MouseX(x),m_MouseY(y){}

	EVENT_CLASS_TYPE(MouseMoved)
	EVENT_CLASS_CATEGORY(EventCategoryMouse|EventCategoryInput)

	std::string ToString() const override {
		std::stringstream ss;
		ss << "MouseMovedEvent: " << m_MouseX << "," << m_MouseY;
		return ss.str();
	}

	inline float GetX() const { return m_MouseX; }
	inline float GetY() const { return m_MouseY; }

	private:
		float m_MouseX, m_MouseY;
	};


	//@brief 鼠标滑轮事件
	class SOLIN_API MouseScrolledEvent :public Event {
	public:
		MouseScrolledEvent(float xOffset,float yOffset)
			:m_XOffset(xOffset),m_YOffset(yOffset){}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << GetXOffset() << "," << GetYOffset();
			return ss.str();
		}

		inline float GetXOffset() const { return m_XOffset; }
		inline float GetYOffset() const { return m_YOffset; }

	private:
		float m_XOffset, m_YOffset;
	};
	
	//@brief 鼠标按钮事件
	class SOLIN_API MouseButtonEvent :public Event {
	public:

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

		inline int GetMouseButton()const { return m_Button; }
	protected:
		int m_Button;
		MouseButtonEvent(int button)
			:m_Button(button){}
	};

	//@brief 鼠标按钮按下事件
	class SOLIN_API MouseButtonPressedEvent :public MouseButtonEvent {
	public:
		MouseButtonPressedEvent(int button)
			:MouseButtonEvent(button){}

		EVENT_CLASS_TYPE(MouseButtonPressed)

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_Button;
			return ss.str();
		}
	};

	//@brief 鼠标按钮释放事件
	class SOLIN_API MouseButtonReleasedEvent :public MouseButtonEvent {
	public:
		MouseButtonReleasedEvent(int button)
			:MouseButtonEvent(button) {}

		EVENT_CLASS_TYPE(MouseButtonReleased)

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonReleaseEvent: " << m_Button;
			return ss.str();
		}
	};

}