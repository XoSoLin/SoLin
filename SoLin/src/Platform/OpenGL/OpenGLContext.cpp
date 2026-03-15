#include "slpch.h"
#include"OpenGLContext.h"

#include"GLFW/glfw3.h"
#include<glad/glad.h>

namespace SoLin {
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		:m_WindowHandle(windowHandle)
	{
		SL_CORE_ASSERT(windowHandle,"Window handle is null !(But it Shouldn't be)")
	}
	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		//通过glad加载自己显卡中OpenGL提供的各种图形渲染函数
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		SL_CORE_ASSERT(status, "Failed to initialize Glad!");
	}
	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}