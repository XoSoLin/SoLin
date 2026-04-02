#include "slpch.h"
#include "WindowsWndow.h"

#include "SoLin\Events\ApplicationEvent.h"
#include "SoLin\Events\KeyEvent.h"
#include "SoLin\Events\MouseEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace SoLin {
	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error_code, const char* description) {
		SL_CORE_ASSERT("GLFW Error ({0}):{1}", error_code, description);
	}

	Window* Window::Create(const WindowProps& props) {
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
        SL_PROFILE_FUNCTION();

		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
        SL_PROFILE_FUNCTION();

		Shutdown();
	}

	void WindowsWindow::OnUpdate()
	{
        SL_PROFILE_FUNCTION();

		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
        SL_PROFILE_FUNCTION();

		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}
	void WindowsWindow::Init(const WindowProps& props)
	{
        SL_PROFILE_FUNCTION();

		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		SL_CORE_INFO("Creating window:{0}({1},{2})", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized) {
            SL_PROFILE_SCOPE("glfwInitWindow");

			//TODO: glfwTerminate on system shutdown

			int success = glfwInit();
			SL_CORE_ASSERT(success, "Could not intialize GLFW!");
			
			// If Initalized successfully but get the wrong on run time
			// 如果初始化启动成功，但是运行时出现错误
			glfwSetErrorCallback(GLFWErrorCallback);

			s_GLFWInitialized = true;
		}

        {
            SL_PROFILE_SCOPE("glfwCreateWindow");
            // 初始化Windows对象并创建窗口上下文
            m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
        }
		m_Context = CreateScope<OpenGLContext>(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		//--------------------------Set GLFW callbacks FN in function "init()"------------------------------------
		//设置GLFW的回调函数
		// ？？？？？？？？？(运行一系列glfw中设置回调函数的函数，以达到引擎中的事件类可以获取到发生对应事件的数据)
		// 得到的这些窗口变化事件的数据，均会生成引擎的事件，再将这些事件传入回调函数，
		// 回调函数中不一定会响应该事件，也就是利用该事件去做其他具体的响应操作。
		// 其中会由事件分发器去过滤有效的事件，然后响应。

		//设置窗口大小回调
		glfwSetWindowSizeCallback(m_Window,
			[](GLFWwindow* window, int width, int height) {
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);	//获取当前窗口的数据指针
				data.Width = width;
				data.Height = height;

				WindowResizeEvent event(width, height);
				data.EventCallback(event);
			}
		);																		//lambda表达式作为第二个参数

		//设置窗口关闭回调
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;
				data.EventCallback(event);
			}
		);

		//设置键盘按键回调
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action) {
				case GLFW_PRESS: { KeyPressedEvent event(key, 0); data.EventCallback(event); break; }
				case GLFW_RELEASE: { KeyReleasedEvent event(key); data.EventCallback(event); break; }
				case GLFW_REPEAT: { KeyPressedEvent event(key, 1); data.EventCallback(event); break; }
				}
			}
		);

		//设置鼠标按键回调
		glfwSetMouseButtonCallback(m_Window,[](GLFWwindow* window,int button,int action,int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action) {
				case GLFW_PRESS: { MouseButtonPressedEvent event(button); data.EventCallback(event); break; }
				case GLFW_RELEASE: { MouseButtonReleasedEvent evnet(button); data.EventCallback(evnet); break; }
				}
			}
		);

		//设置鼠标滑轮回调
		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseScrolledEvent event((float)xOffset, (float)yOffset);
				data.EventCallback(event);
			}
		);

		//设置鼠标移动回调
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window,double xpos,double ypos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseMovedEvent event((float)xpos, (float)ypos);
				data.EventCallback(event);
			}
		);

		//设置键盘输入回调
		glfwSetCharCallback(m_Window, [](GLFWwindow* window,unsigned int keycode)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				KeyTypedEvent event(keycode);
				data.EventCallback(event);
			}
		);

	}
	void WindowsWindow::Shutdown()
	{
        SL_PROFILE_FUNCTION();

		glfwDestroyWindow(m_Window);
	}
}
