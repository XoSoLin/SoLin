#pragma once

#include"Event.h"

namespace SoLin {

	// @brief 按键事件基类
	class SOLIN_API KeyEvent :public Event {
	public:
		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)			// 一个位代表一个类别，传入两个类别将会占用对应的两个位

		inline int GetKeyCode() const { return m_KeyCode; }
	protected:
		int m_KeyCode;
		KeyEvent(int keycode)
			:m_KeyCode(keycode) {}
	};

	// @brief 按键按下事件
	class SOLIN_API KeyPressedEvent :public KeyEvent {
	public:
		KeyPressedEvent(int keycode, int repeatCount) 
			:KeyEvent(keycode),m_RepeatCount(repeatCount)
		{}

		EVENT_CLASS_TYPE(KeyPressed)

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << "(" << m_RepeatCount << ")";
			return ss.str();
		}

		inline int GetRepeatCount() const { return m_RepeatCount; }
	private:
		int m_RepeatCount;
	};

	// @brief 按键释放事件
	class SOLIN_API KeyReleasedEvent :public KeyEvent {
	public:
		KeyReleasedEvent(int keycode)
			:KeyEvent(keycode)
		{}

		EVENT_CLASS_TYPE(KeyReleased)

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}
	};

	class SOLIN_API KeyTypedEvent :public KeyEvent {
	public:
		KeyTypedEvent(int keycode)
			:KeyEvent(keycode)
		{}

		EVENT_CLASS_TYPE(KeyTyped)

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}
	};

	// TO ADD Key Event
}