#include "slpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"
#include "Platform/OpenGL/ImGuiGLFW.h"
#include "GLFW/glfw3.h"

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

		ImGui_ImplGlfw_InitForOpenGL(current_window, true);

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
	}
}
