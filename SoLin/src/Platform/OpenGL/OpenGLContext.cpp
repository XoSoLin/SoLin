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

		SL_CORE_INFO("OpenGL info: ");
		SL_CORE_INFO("	Vendor: {0}", (const char*)glGetString(GL_VENDOR));				//使用的GPU厂商
		SL_CORE_INFO("	Renderer: {0}", (const char*)glGetString(GL_RENDERER));			//使用的GPU渲染器
		SL_CORE_INFO("	Version: {0}", (const char*)glGetString(GL_VERSION));			//渲染版本号
	}
	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}