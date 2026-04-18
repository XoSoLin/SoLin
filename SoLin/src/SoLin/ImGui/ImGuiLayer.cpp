#include "slpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include"SoLin/Core/Application.h"

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<ImGuizmo.h>

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

        // 修改默认字体
        io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/VonwaonBitmap-16px.ttf", 16.0f);

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

        // 暂时设定为粉红色，后续完善样式系统
        SetThemeColors(ThemeColor::Red);

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

    void ImGuiLayer::OnEvent(Event& e)
    {
        if (m_BlockEvents) {
            ImGuiIO& io = ImGui::GetIO();

            // 当Imgui层想要处理鼠标与键盘输入时，将会消耗掉事件
            e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
            e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
        }
    }

	void ImGuiLayer::Begin() {
        SL_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_NewFrame();										//每一帧开始时准备 OpenGL 渲染环境以供 Dear ImGui 绘制 UI 元素
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();													//清除之前的 UI 数据，准备接受新的 UI 绘制指令（准备新帧），并更新输入状态

        ImGuizmo::BeginFrame();
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

    void ImGuiLayer::SetThemeColors(ThemeColor type)
    {
        switch (type) {
        case ThemeColor::Dark:
            SetDarkThemeColors();
            break;
        case ThemeColor::Blue:
            SetBlueThemeColors();
            break;
        case ThemeColor::Red:
            SetRedThemeColors();
            break;
        default:
            break;
        }
    }

    void ImGuiLayer::SetDarkThemeColors()
    {
// -----------------------------------灰黑-----------------------------------------------
        auto& colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };
        // Headers
        colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Buttons
        colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        // Frame BG
        colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        // Tabs
        colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
        colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
        colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        // Title
        colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    }

    void ImGuiLayer::SetBlueThemeColors()
    {
        //---------------------------------- - 蓝白---------------------------------------------- -
            auto& colors = ImGui::GetStyle().Colors;

            // 1. 基础面板颜色 (明亮/白色系)
            colors[ImGuiCol_WindowBg] = ImVec4{ 0.94f, 0.94f, 0.94f, 1.00f };  // 主背景: 浅灰白
            colors[ImGuiCol_ChildBg] = ImVec4{ 0.94f, 0.94f, 0.94f, 1.00f };
            colors[ImGuiCol_PopupBg] = ImVec4{ 1.00f, 1.00f, 1.00f, 1.00f };    // 弹出菜单: 纯白

            // 2. 控件框架 (Frame) - 浅色系
            colors[ImGuiCol_FrameBg] = ImVec4{ 1.00f, 1.00f, 1.00f, 1.00f };          // 普通: 纯白
            colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.90f, 0.95f, 1.00f, 1.00f };    // 悬停: 淡蓝白
            colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.80f, 0.88f, 1.00f, 1.00f };     // 激活: 浅蓝

            // 3. 按钮 (Button) - 蓝色主题
            colors[ImGuiCol_Button] = ImVec4{ 0.20f, 0.55f, 0.90f, 1.00f };            // 普通: 亮蓝
            colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.25f, 0.65f, 1.00f, 1.00f };     // 悬停: 更亮蓝
            colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.45f, 0.80f, 1.00f };      // 按下: 深蓝

            // 4. 头部/页签 (Header/Tab) - 蓝色主题
            colors[ImGuiCol_Header] = ImVec4{ 0.80f, 0.88f, 1.00f, 1.00f };            // 普通: 浅蓝
            colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.70f, 0.80f, 1.00f, 1.00f };     // 悬停: 中浅蓝
            colors[ImGuiCol_HeaderActive] = ImVec4{ 0.60f, 0.70f, 0.95f, 1.00f };      // 激活: 稍深蓝

            colors[ImGuiCol_Tab] = ImVec4{ 0.85f, 0.90f, 0.95f, 1.00f };               // 非激活页签: 浅灰蓝
            colors[ImGuiCol_TabHovered] = ImVec4{ 0.70f, 0.80f, 1.00f, 1.00f };        // 悬停页签: 浅蓝
            colors[ImGuiCol_TabActive] = ImVec4{ 0.20f, 0.55f, 0.90f, 1.00f };         // 激活页签: 亮蓝 (高亮)
            colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.85f, 0.88f, 0.90f, 1.00f };
            colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.60f, 0.70f, 0.80f, 1.00f };

            // 5. 标题栏 (Title)
            colors[ImGuiCol_TitleBg] = ImVec4{ 0.85f, 0.88f, 0.92f, 1.00f };           // 非激活: 浅灰蓝
            colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.20f, 0.55f, 0.90f, 1.00f };     // 激活: 亮蓝
            colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.85f, 0.88f, 0.92f, 1.00f };

            // 6. 文字颜色 (关键: 深色以保证可读性)
            colors[ImGuiCol_Text] = ImVec4{ 0.10f, 0.10f, 0.10f, 1.00f };              // 主要文字: 深黑
            colors[ImGuiCol_TextDisabled] = ImVec4{ 0.60f, 0.60f, 0.60f, 1.00f };      // 禁用文字: 中灰
            colors[ImGuiCol_TextSelectedBg] = ImVec4{ 0.20f, 0.55f, 0.90f, 1.00f };    // 选中文字背景

            // 7. 分隔线/边框 (Border)
            colors[ImGuiCol_Border] = ImVec4{ 0.70f, 0.75f, 0.80f, 1.00f };
            colors[ImGuiCol_Separator] = ImVec4{ 0.70f, 0.75f, 0.80f, 1.00f };
            colors[ImGuiCol_SeparatorHovered] = ImVec4{ 0.20f, 0.55f, 0.90f, 1.00f };
            colors[ImGuiCol_SeparatorActive] = ImVec4{ 0.15f, 0.45f, 0.80f, 1.00f };

            // 8. 滑块/滚动条 (Slider/Scrollbar)
            colors[ImGuiCol_SliderGrab] = ImVec4{ 0.20f, 0.55f, 0.90f, 1.00f };
            colors[ImGuiCol_SliderGrabActive] = ImVec4{ 0.15f, 0.45f, 0.80f, 1.00f };
            colors[ImGuiCol_ScrollbarGrab] = ImVec4{ 0.70f, 0.75f, 0.85f, 1.00f };
            colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4{ 0.60f, 0.65f, 0.80f, 1.00f };
            colors[ImGuiCol_ScrollbarGrabActive] = ImVec4{ 0.50f, 0.55f, 0.70f, 1.00f };

            // 9. 其他 (CheckMark, 拖动线等)
            colors[ImGuiCol_CheckMark] = ImVec4{ 0.20f, 0.55f, 0.90f, 1.00f };
            colors[ImGuiCol_DragDropTarget] = ImVec4{ 0.20f, 0.55f, 0.90f, 0.60f };
    }

    void ImGuiLayer::SetRedThemeColors()
    {
        // ---------------------------------- - 粉红---------------------------------------------- -
        auto& colors = ImGui::GetStyle().Colors;

        // 1. 基础面板颜色 (柔和粉白系)
        colors[ImGuiCol_WindowBg] = ImVec4{ 0.98f, 0.94f, 0.96f, 1.00f };  // 主背景: 极浅粉白
        colors[ImGuiCol_ChildBg] = ImVec4{ 0.98f, 0.94f, 0.96f, 1.00f };
        colors[ImGuiCol_PopupBg] = ImVec4{ 1.00f, 0.96f, 0.98f, 1.00f };    // 弹出菜单: 浅粉白

        // 2. 控件框架 (Frame) - 浅粉系
        colors[ImGuiCol_FrameBg] = ImVec4{ 1.00f, 0.96f, 0.98f, 1.00f };          // 普通: 粉白
        colors[ImGuiCol_FrameBgHovered] = ImVec4{ 1.00f, 0.88f, 0.94f, 1.00f };    // 悬停: 浅粉
        colors[ImGuiCol_FrameBgActive] = ImVec4{ 1.00f, 0.80f, 0.90f, 1.00f };     // 激活: 樱花粉

        // 3. 按钮 (Button) - 粉红主题
        colors[ImGuiCol_Button] = ImVec4{ 0.95f, 0.50f, 0.65f, 1.00f };            // 普通: 珊瑚粉
        colors[ImGuiCol_ButtonHovered] = ImVec4{ 1.00f, 0.55f, 0.70f, 1.00f };     // 悬停: 亮珊瑚粉
        colors[ImGuiCol_ButtonActive] = ImVec4{ 0.85f, 0.40f, 0.55f, 1.00f };      // 按下: 深玫瑰粉

        // 4. 头部/页签 (Header/Tab) - 粉红主题
        colors[ImGuiCol_Header] = ImVec4{ 1.00f, 0.80f, 0.88f, 1.00f };            // 普通: 淡粉
        colors[ImGuiCol_HeaderHovered] = ImVec4{ 1.00f, 0.70f, 0.82f, 1.00f };     // 悬停: 樱花粉
        colors[ImGuiCol_HeaderActive] = ImVec4{ 0.95f, 0.60f, 0.75f, 1.00f };      // 激活: 玫瑰粉

        colors[ImGuiCol_Tab] = ImVec4{ 0.95f, 0.85f, 0.90f, 1.00f };               // 非激活页签: 浅灰粉
        colors[ImGuiCol_TabHovered] = ImVec4{ 1.00f, 0.70f, 0.85f, 1.00f };        // 悬停页签: 亮粉
        colors[ImGuiCol_TabActive] = ImVec4{ 0.95f, 0.50f, 0.65f, 1.00f };         // 激活页签: 珊瑚粉 (高亮)
        colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.92f, 0.88f, 0.90f, 1.00f };
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.85f, 0.70f, 0.75f, 1.00f };

        // 5. 标题栏 (Title)
        colors[ImGuiCol_TitleBg] = ImVec4{ 0.95f, 0.85f, 0.90f, 1.00f };           // 非激活: 浅粉灰
        colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.95f, 0.55f, 0.70f, 1.00f };     // 激活: 珊瑚粉
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.95f, 0.85f, 0.90f, 1.00f };

        // 6. 文字颜色 (深灰色保证可读性)
        colors[ImGuiCol_Text] = ImVec4{ 0.25f, 0.20f, 0.22f, 1.00f };              // 主要文字: 深灰(带一点紫)
        colors[ImGuiCol_TextDisabled] = ImVec4{ 0.65f, 0.60f, 0.62f, 1.00f };      // 禁用文字: 中灰
        colors[ImGuiCol_TextSelectedBg] = ImVec4{ 0.95f, 0.50f, 0.65f, 1.00f };    // 选中文字背景

        // 7. 分隔线/边框 (Border) - 浅粉灰
        colors[ImGuiCol_Border] = ImVec4{ 0.85f, 0.80f, 0.82f, 1.00f };
        colors[ImGuiCol_Separator] = ImVec4{ 0.85f, 0.80f, 0.82f, 1.00f };
        colors[ImGuiCol_SeparatorHovered] = ImVec4{ 0.95f, 0.50f, 0.65f, 1.00f };
        colors[ImGuiCol_SeparatorActive] = ImVec4{ 0.85f, 0.40f, 0.55f, 1.00f };

        // 8. 滑块/滚动条 (Slider/Scrollbar) - 粉红主题
        colors[ImGuiCol_SliderGrab] = ImVec4{ 0.95f, 0.50f, 0.65f, 1.00f };
        colors[ImGuiCol_SliderGrabActive] = ImVec4{ 0.85f, 0.40f, 0.55f, 1.00f };
        colors[ImGuiCol_ScrollbarGrab] = ImVec4{ 0.85f, 0.75f, 0.80f, 1.00f };
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4{ 0.90f, 0.70f, 0.80f, 1.00f };
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4{ 0.80f, 0.60f, 0.70f, 1.00f };

        // 9. 其他 (CheckMark, 拖动线等)
        colors[ImGuiCol_CheckMark] = ImVec4{ 0.95f, 0.50f, 0.65f, 1.00f };
        colors[ImGuiCol_DragDropTarget] = ImVec4{ 0.95f, 0.50f, 0.65f, 0.60f };
    }

}
