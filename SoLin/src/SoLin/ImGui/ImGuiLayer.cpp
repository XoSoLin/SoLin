#include "slpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include"SoLin/Core/Application.h"

#include<glad/glad.h>
#include<GLFW/glfw3.h>

namespace SoLin {
	ImGuiLayer::ImGuiLayer()
		:Layer("ImGui_Layer")
	{}

	ImGuiLayer::~ImGuiLayer(){}

	void ImGuiLayer::OnAttach() {
        SL_PROFILE_FUNCTION();

		SL_CORE_INFO("void ImGuiLayer::OnAttach");

		IMGUI_CHECKVERSION();									//初始化ImGui核心

		ImGui::CreateContext();

		//ImGui::StyleColorsDark();								//初始化样式（深/  ）
		ImGui::StyleColorsLight();								//				 浅

		ImGuiIO& io = ImGui::GetIO();							//创建用于输入输出的对象
		(void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;	//启用键盘导航
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		//初始化GLFW后端（关键：让ImGui原生识别GLFW keycode）
		//核心：用glfwGetCurrentContext()获取当前窗口句柄（无需m_Window）
		//GLFWwindow* current_window = glfwGetCurrentContext();
		Application& app = Application::Get();
		GLFWwindow* current_window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		ImGui_ImplGlfw_InitForOpenGL(current_window, true);		//此处便可以对ImGui的demo窗口进行事件响应，但并不是响应本引擎封装的Event
																				//相当于ImGui像本引擎一样，已经从窗口获得了信息,否则需要将引擎得到的信息转递给ImGui的io

		ImGui_ImplOpenGL3_Init("#version 410");		//初始化 ImGui 在 OpenGL3 或更高版本环境下渲染器，并使用GLSL 4.10 版本进行着色器编译和处理
	}

	void ImGuiLayer::OnDetach() {
        SL_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::Begin() {
        SL_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_NewFrame();										//每一帧开始时准备 OpenGL 渲染环境以供 Dear ImGui 绘制 UI 元素
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();													//清除之前的 UI 数据，准备接受新的 UI 绘制指令（准备新帧），并更新输入状态
	}

	void ImGuiLayer::OnImGuiRender() {
        SL_PROFILE_FUNCTION();

		static bool show = true;
		//ImGui::ShowDemoWindow(&show);									//展示Demo窗口，内置的一个ImGui窗口
	}

	void ImGuiLayer::End() {
        SL_PROFILE_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		ImGui::Render();													//结束当前帧的 UI 绘制过程，并准备将绘制的数据保存到暂存区
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());		//将缓冲区中的绘制数据提交给渲染器进行操作

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

}
