#pragma once

#include "slpch.h"
#include "SoLin\Core\Core.h"

namespace SoLin {

//枚举-------------------------------------------------------------------------------

	//@brief 事件类型
	enum class EventType {
		None = 0,
		WindowClose,WindowResize,WindowFocus,WindowLostFocus,WindowMoved,
		AppTick,AppUpdate,AppRender,
		KeyPressed,KeyReleased,KeyTyped,
		MouseButtonPressed,MouseButtonReleased,MouseScrolled,MouseMoved
	};
	// enum class使用时有命名空间限制

	//@brief 事件类别 
	//@note 用于对事件进行过滤
	enum EventCategory {
		None = 0,
		EventCategoryApplication = Bit(0),
		EventCategoryInput = Bit(1),
		EventCategoryKeyboard = Bit(2),
		EventCategoryMouse = Bit(3),
		EventCategoryMouseButton = Bit(4)
	};

//宏---------------------------------------------------------------------------------------

	//@brief 自动化以下的重写操作
	//@brief 其中 # 为字符串化操作符，放在宏参数前使其变为字符串字面量
	//@brief 其中 ## 为标记粘贴操作符，将宏参数与其他文本组合成一个新的标识符
#define EVENT_CLASS_TYPE(type)\
		static EventType GetStaticType(){return EventType::##type;}\
		virtual EventType GetEventType() const override{return GetStaticType();}\
		virtual const char* GetName() const override {return #type;}
	
	//@brief 自动化以下的重写操作
#define EVENT_CLASS_CATEGORY(category)\
		virtual int GetCategoryFlags() const override {return category;}

//类-------------------------------------------------------------------------------------------
	
	//@class Event
	//@brief 事件基类
	class SOLIN_API Event {
		// 声明事件分发器为友元
		friend class EventDispatcher;
	public:
		// 是否已经处理
		bool Handled = false;
		// @brief 用于获取事件类型
		virtual EventType GetEventType() const = 0;
		// @brief 用于获取事件名称的字符串
		virtual const char* GetName() const = 0;
		// @brief 用于获取事件类别标识
		virtual int GetCategoryFlags() const = 0;
		// @brief 用于流传输自身信息，与重载的<<一起使用
		virtual std::string ToString() const { return GetName(); }

		//@brief 用于判断当前事件是否与给定事件同类
		//@param category 事件类别枚举
		inline bool IsInCategory(EventCategory category) {
			return GetCategoryFlags() & category;							//因为类别是由位决定，&操作后有着对应位的类型才会被判断成属于一种类型。也就是会存在1，否则全0
		}
	protected:
	};

	//@class EventDispatcher
	//@brief 事件分发器
	class EventDispatcher {
	private:
		Event& m_Event;

		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event)
			:m_Event(event) {

		}
		template<typename T>													//为Dispatch函数提供需要使用的模板T（本来可以只写一个template，但是EventFn是私有的，和公有的区分开）
		bool Dispatch(EventFn<T> func) {										//接受一个“参数为T”且“返回值为bool”的可调度表达式func （函数指针、lambda）
			if (m_Event.GetEventType() == T::GetStaticType()) {					//！！！静态函数在使用时需要使用类名或类型名来调用
				m_Event.Handled = func(*(T*)&m_Event);							//*(T*) 表示：用 * 解引用（T*）所声明的T类型指针，实现强制类型转换
																				//？？？？？？意思就是此刻的&m_Event的类型	就应该是&T	也就是*(T*)
																				// func();也就执行了对应的函数，并将bool返回给m_Handled
				return true;
			}
			return false;
		}
	};

	//@brief 重载<<运算符
	//@param os 输出流引用
	//@param ev 事件引用
	inline std::ostream& operator<<(std::ostream& os, const Event& ev) {
		return os << ev.ToString();
	}
}