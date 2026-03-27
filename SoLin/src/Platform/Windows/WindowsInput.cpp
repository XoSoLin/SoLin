#include "slpch.h"
#include"WindowsInput.h"

#include"SoLin/Core/Application.h"

#include<GLFW/glfw3.h>

namespace SoLin {
	Input* Input::s_Instance = new WindowsInput();		//s_Instance 是一个没有包含任何数据的实例对象

	bool WindowsInput::IsKeyPressedImpl(int keycode) {
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		//.Get()返回Application这个单例中的*s_Instance实例 .GetWindow()返回Application类中的成员m_Window（Windows类的） .GetNativeWindow()返回Window类中的成员GLFWwindow

		auto state = glfwGetKey(window, keycode);
		return state == (GLFW_PRESS || GLFW_REPEAT);		//（按下和重复都算做是按下了）
	}
	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetMouseButton(window, button);

		return state == GLFW_PRESS;
	}
	std::pair<float, float> WindowsInput::GetMousePosImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window,&xpos,&ypos);
		return {(float)xpos,(float)ypos};
	}
	float WindowsInput::GetMouseXImpl()
	{
		auto [x, y] = GetMousePosImpl();
		return x;
	}
	float WindowsInput::GetMouseYImpl()
	{
		auto [x, y] = GetMousePosImpl();
		return y;
	}
}