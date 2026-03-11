#include "slpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include"Platform/OpenGL/UseImGui.h"

#include"SoLin/Application.h"

namespace SoLin {
	ImGuiLayer::ImGuiLayer()
		:Layer("ImGui_Layer")
	{}

	ImGuiLayer::~ImGuiLayer(){}

	void ImGuiLayer::OnAttach() {

		SL_CORE_INFO("void ImGuiLayer::OnAttach");

		IMGUI_CHECKVERSION();									//初始化ImGui核心

		ImGui::CreateContext();

		//ImGui::StyleColorsDark();								//初始化样式（深/  ）
		ImGui::StyleColorsLight();								//				 浅

		ImGuiIO& io = ImGui::GetIO();							//创建用于输入输出的对象
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;	//后端支持自定义鼠标指针，并告诉 Daer Imgui 开启鼠标指针功能
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;	//后端支持设置鼠标位置，并告诉 Daer Imgui 开启设置鼠标位置的功能 
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;	//启用键盘导航

		//初始化GLFW后端（关键：让ImGui原生识别GLFW keycode）
		//核心：用glfwGetCurrentContext()获取当前窗口句柄（无需m_Window）
		GLFWwindow* current_window = glfwGetCurrentContext();

		//ImGui_ImplGlfw_InitForOpenGL(current_window, true);		//此处便可以对ImGui的demo窗口进行事件响应，但并不是响应本引擎封装的Event
																				//相当于ImGui像本引擎一样，已经从窗口获得了信息,否则需要将引擎得到的信息转递给ImGui的io

		ImGui_ImplOpenGL3_Init("#version 410");		//初始化 ImGui 在 OpenGL3 或更高版本环境下渲染器，并使用GLSL 4.10 版本进行着色器编译和处理
	}

	void ImGuiLayer::OnDetach() {

	}

	void ImGuiLayer::OnUpdate()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		float time = (float)glfwGetTime();
		io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
		m_Time = time;

		ImGui_ImplOpenGL3_NewFrame();										//每一帧开始时准备 OpenGL 渲染环境以供 Dear ImGui 绘制 UI 元素
		ImGui::NewFrame();													//清除之前的 UI 数据，准备接受新的 UI 绘制指令（准备新帧），并更新输入状态

		static bool show = true;
		ImGui::ShowDemoWindow(&show);									//展示Demo窗口，内置的一个ImGui窗口

		ImGui::Render();													//结束当前帧的 UI 绘制过程，并准备将绘制的数据保存到暂存区
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());		//将缓冲区中的绘制数据提交给渲染器进行操作
		
	}

	void ImGuiLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		//此处相当于将原本传入的event内含有的信息转发给了ImGui的io
		//对应事件全部返回false是因为仅仅传递信息，不做响应
		dispatcher.Dispatch<WindowResizeEvent>(SOLIN_BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));
		dispatcher.Dispatch<MouseMovedEvent>(SOLIN_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
		dispatcher.Dispatch<MouseScrolledEvent>(SOLIN_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
		dispatcher.Dispatch<MouseButtonPressedEvent>(SOLIN_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(SOLIN_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
		dispatcher.Dispatch<KeyTypedEvent>(SOLIN_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
		dispatcher.Dispatch<KeyPressedEvent>(SOLIN_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(SOLIN_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));

		SL_CORE_INFO("{0},{1}", event.ToString(),event.Handled);
	}

	bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		glViewport(0.0f, 0.0f, e.GetWidth(), e.GetHeight());

		return false;
	}

	//鼠标移动信息传递给ImGui的io
	bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.GetX(), e.GetY());		//传入鼠标位置到IO中

		return false;
	}

	//鼠标滑轮信息附加给ImGui的io
	bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += e.GetXOffset();				//水平方向（horizontal）
		io.MouseWheel += e.GetYOffset();				//垂直方向

		return false;
	}

	//鼠标按下信息传递给ImGui的io
	bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = true;		//将对应鼠标按钮的状态设为 true,表示按下

		return false;
	}

	//鼠标释放信息转递给ImGui的io
	bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = false;		//将对应鼠标按钮的状态设为 false,表示释放

		return false;
	}

	bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		int keycode = e.GetKeyCode();
		if (keycode > 0 && keycode < 0x10000)
			io.AddInputCharacter((unsigned short)keycode); 

		return false;
	}

	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGuiKey imguiKey = keyGlToImGui(e.GetKeyCode());		

		if (imguiKey != ImGuiKey_None) {
			io.AddKeyEvent(imguiKey, true);	//将对应键盘按键的状态设为 true,表示按下
		}
		

		//io.KeyCtrl = io.KeysDown[GLFW_KEY_RIGHT_CONTROL] || io.KeysDown[GLFW_KEY_LEFT_CONTROL];
		//io.KeyShift = io.KeysDown[GLFW_KEY_RIGHT_SHIFT] || io.KeysDown[GLFW_KEY_LEFT_SHIFT];
		//io.KeyAlt = io.KeysDown[GLFW_KEY_RIGHT_ALT] || io.KeysDown[GLFW_KEY_LEFT_ALT];
		//io.KeySuper = io.KeysDown[GLFW_KEY_RIGHT_SUPER] || io.KeysDown[GLFW_KEY_LEFT_SUPER];

		return false;
	}

	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGuiKey imguiKey = keyGlToImGui(e.GetKeyCode());

		if (imguiKey != ImGuiKey_None) {
			io.AddKeyEvent(imguiKey, false);	//将对应键盘按键的状态设为 false,表示释放
		}	

		return false;
	}

}
