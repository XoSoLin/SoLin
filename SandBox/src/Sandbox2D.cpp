#include "Sandbox2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static const char* s_GameMap = {
    "AAAAAAAAAAAAAAAAAMMA"
    "AAAAAAAAAAAAAAAALLLL"
    "AAAAAAAAAAAAAAAADDDD"
    "SAAAAAAAAAAAAAAAAAAA"
    "LWWWWLAAAAAAAAAAABAA"
    "LWWWWLLLLAAAAAAAAAAA"
    "DDDDDDDDDAAAALLLLGGL"
    "AAAAAAAAAAAAADDDDDDD"
    "AAAAAAAAAAAAAAAAAAAA"
    "AAALLLLAALLLLLLLLLLL"
    "GGGDDDDGGDDDDDDDDDDD"
    "DDDDDDDDDDDDDDDDDDDD" };
static const  uint32_t s_MapWidth = 20;
static const  uint32_t s_MapHeight = strlen(s_GameMap) / s_MapWidth;

Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"),m_CameraController(1280.0f/720.0f,true)
{
}

void Sandbox2D::OnAttach()
{
    SL_PROFILE_FUNCTION();

    m_Framebuffer = SoLin::FrameBuffer::Create({ 1280,720 });

	//文件编码转utf-8后在vs中运行时utf-8会被当作gb2312去处理，有中文时会导致程序运行时有乱码
	m_Texture = SoLin::Texture2D::Create(SLPATH("assets/textures/千夏02.png"));

    m_SpriteSheet = SL::Texture2D::Create("assets/textures/tilemap_packed .png");
    m_TilesMap['D'] = SL::SubTexture2D::Create(m_SpriteSheet, { 18, 18 }, { 4, 8 } /* ,{1,1} */);
    m_TilesMap['G'] = SL::SubTexture2D::Create(m_SpriteSheet, { 18, 18 }, { 0, 8 });
    m_TilesMap['L'] = SL::SubTexture2D::Create(m_SpriteSheet, { 18, 18 }, { 0, 6 });
    m_TilesMap['W'] = SL::SubTexture2D::Create(m_SpriteSheet, { 18, 18 }, { 13, 5 });
    m_TilesMap['B'] = SL::SubTexture2D::Create(m_SpriteSheet, { 18, 18 }, { 9, 8 });
    m_TilesMap['K'] = SL::SubTexture2D::Create(m_SpriteSheet, { 18, 18 }, { 7, 7 });
    m_TilesMap['F'] = SL::SubTexture2D::Create(m_SpriteSheet, { 18, 18 }, { 11, 3 });
    m_TilesMap['M'] = SL::SubTexture2D::Create(m_SpriteSheet, { 18, 18 }, { 8, 2 });
    m_TilesMap['S'] = SL::SubTexture2D::Create(m_SpriteSheet, { 18, 18 }, { 5, 1 });

    m_SpriteSheetRole = SL::Texture2D::Create("assets/textures/tilemap_packed(2).png");
    m_Role = SL::SubTexture2D::Create(m_SpriteSheetRole, { 16, 16 }, { 0, 3 }, { 1, 1 });

#pragma region Particle Init here
    m_Particle.ColorBegin = { 138 / 255.0f, 43 / 255.0f, 226 / 255.0f, 1.0f };
    m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
    m_Particle.SizeBegin = 0.3f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
    m_Particle.LifeTime = 1.0f;
    m_Particle.Velocity = { 0.0f, 0.0f };
    m_Particle.VelocityVariation = { 3.0f, 1.0f };
    m_Particle.Position = { 0.0f, 0.0f };
#pragma endregion

    m_CameraController.SetZoomLevel(5.0f);

}

void Sandbox2D::OnDetach()
{
    SL_PROFILE_FUNCTION();

}

void Sandbox2D::OnUpdate(SoLin::Timestep ts)
{
    SL_PROFILE_FUNCTION();
	//Update
	m_CameraController.OnUpdate(ts);
	//Render
	{
		SL_PROFILE_SCOPE("RenderCommand Prep");
        m_Framebuffer->Bind();
		SoLin::RendererCommand::SetClearColor({ 0.1f,0.1f,0.1f,1 });
		SoLin::RendererCommand::Clear();
	}
		
	{
		SL_PROFILE_SCOPE("Renderer2D Draw");

#if 1
        static float temp = 0.0f;
        temp += ts * 100.0f;

		//这里会消耗很多时间,改了垂直同步后快了，
		// 测试后应该是设置统一变量影响到这里的，是与OpenGL帧率控制相关
		SoLin::Renderer2D::BeginScene(m_CameraController.GetCamera());
		SoLin::Renderer2D::DrawQuad({ 0.0f,0.0f }, { 1.0f, 1.0f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		SoLin::Renderer2D::DrawQuad({ 1.0f, -1.0f,-0.2 }, { 0.5f, 1.0f }, m_SquareColor);
		//通过opengl设置后，纹理坐标超出-1到1的部分，会重复显示
        SoLin::Renderer2D::DrawRotatedQuad({ -1.0f, 1.0f }, { 1.0f, 1.0f }, glm::radians(temp), { 0.3f, 0.2f, 0.8f, 1.0f });
		SoLin::Renderer2D::DrawQuad({ 0.0f,0.0f,-0.1f }, { 2.0f,2.0f }, m_Texture,10.0f,{1.0,0.9,0.9,1.0});
		SoLin::Renderer2D::EndScene();

        SL::Renderer2D::BeginScene(m_CameraController.GetCamera());
        for (float y = -5.0f; y < 5.0f; y += 0.5f)
        {
            for (float x = -5.0f; x < 5.0f; x += 0.5f)
            {
                glm::vec4 color = { 0.0f ,(x + 5.0f) / 10.0f, (y + 5.0f) / 10.0f, 0.7f };
                SL::Renderer2D::DrawQuad({ x,y }, { 0.45f, 0.45f }, color);
            }
        }
        SL::Renderer2D::EndScene();
#endif

#if 1
        SL::Renderer2D::BeginScene(m_CameraController.GetCamera());
        if (SL::Input::IsMouseButtonPressed(SL_MOUSE_BUTTON_LEFT))
        {
            auto [x, y] = SL::Input::GetMousePos();							// 获取窗口中鼠标当前的位置
            auto width = SL::Application::Get().GetWindow().GetWidth();		// 获取窗口宽高
            auto height = SL::Application::Get().GetWindow().GetHeight();

            auto bounds = m_CameraController.GetBounds();						// 获取相机边界（相机的视景体->平截头体），即相机能观察到的空间区域
            auto pos = m_CameraController.GetCamera().GetPosition();			// 获取相机在空间中的位置
            x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
            y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
            m_Particle.Position = { x + pos.x, y + pos.y };
            for (int i = 0; i < 1; i++)
                m_ParticleSystem.Emit(m_Particle);
        }
        m_ParticleSystem.OnUpdate(ts);											// 更新信息
        m_ParticleSystem.OnRender(m_CameraController.GetCamera());				// 渲染粒子
        SL::Renderer2D::EndScene();

        SL::Renderer2D::BeginScene(m_CameraController.GetCamera());

        for (uint32_t x = 0; x < s_MapWidth; x++) {
            for (uint32_t y = 0; y < s_MapHeight; y++)
            {
                char keyChar = s_GameMap[x + y * s_MapWidth];

                SL::Ref<SL::SubTexture2D> subTexture;
                if (m_TilesMap.find(keyChar) != m_TilesMap.end()) {
                    subTexture = m_TilesMap[keyChar];
                    SL::Renderer2D::DrawQuad({ x, s_MapHeight - y , 0.0f }, { 1.0f, 1.0f }, subTexture);
                }
                else
                    SL::Renderer2D::DrawQuad({ x, s_MapHeight - y, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 1.0f });
            }
        }
        SL::Renderer2D::DrawQuad({ 18.0f, 7.0f, 0.5f }, { 1.0f, 1.0f }, m_Role);

        SL::Renderer2D::EndScene();
#endif
        m_Framebuffer->UnBind();
	}
}

void Sandbox2D::OnImGuiRender()
{
    SL_PROFILE_FUNCTION();

    static bool dockingEnable = true;
    if (dockingEnable) {
        static bool dockspaceOpen = true;                                       // 停靠空间开启
        static bool opt_fullscreen = true;                                      // 填充窗口屏幕
        static bool opt_padding = false;                                        // 内边框
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;    // 停靠结点行为标志

        // ImGuiWindowFlags_MenuBar：允许窗口拥有菜单栏
        // ImGuiWindowFlags_NoDocking：父窗口本身不可被停靠，避免嵌套停靠造成混乱
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();           // 获取主视口(显示器工作区)
            ImGui::SetNextWindowPos(viewport->WorkPos);                         // 设置窗口位置为工作区起点
            ImGui::SetNextWindowSize(viewport->WorkSize);                       // 设置窗口大小为工作区大小
            ImGui::SetNextWindowViewport(viewport->ID);                         // 绑定到该视口
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);            // 设置样式为无圆角
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);          // 设置样式为无边框
            window_flags |= ImGuiWindowFlags_NoTitleBar                         // 无标题栏
                | ImGuiWindowFlags_NoCollapse                                   // 不能折叠
                | ImGuiWindowFlags_NoResize                                     // 不能调整大小
                | ImGuiWindowFlags_NoMove;                                      // 不能移动
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus              // 聚焦时不前置
                | ImGuiWindowFlags_NoNavFocus;                                  // 不获取导航焦点
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        // 在使用ImGuiDockNodeFlags_PassthruCentralNode时，DockSpace()会渲染我们的背景
        // 并处理穿透孔，因此我们要求Begin()不要渲染背景。
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        if (!opt_padding)
            //移除窗口内边距，让停靠空间完全填满。
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);

        if (!opt_padding)
            ImGui::PopStyleVar();   // 弹出ImGuiStyleVar_WindowPadding

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);  // 弹出ImGuiStyleVar_WindowRounding和ImGuiStyleVar_WindowBorderSize

        // 提交停靠空间
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Options"))
            {
                // 禁用全屏模式将允许窗口移动到其他窗口的前面，
                // 目前我们无法在没有更精细的窗口深度/Z控制的情况下撤销此操作。
                ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
                ImGui::MenuItem("Padding", NULL, &opt_padding);
                ImGui::Separator();

                /*if (ImGui::MenuItem("Flag: NoDockingOverCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingOverCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingOverCentralNode; }
                if (ImGui::MenuItem("Flag: NoDockingSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingSplit; }
                if (ImGui::MenuItem("Flag: NoUndocking", "", (dockspace_flags & ImGuiDockNodeFlags_NoUndocking) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoUndocking; }
                if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
                if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
                if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }*/
                if (ImGui::MenuItem("Exit")) { SoLin::Application::Get().WindowClose(); }

                /*ImGui::Separator();
                if (ImGui::MenuItem("Close", NULL, false))
                    dockspaceOpen = false;*/

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

	    ImGui::Begin("SandboxTest");
        auto stats = SL::Renderer2D::GetStats();
        ImGui::Text("Renderer2D Stats:");
        ImGui::Text("Draw Calls: %d", stats.DrawCalls);
        ImGui::Text("Quads: %d", stats.QuadCount);
        ImGui::Text("Vertices: %d", stats.GetVertexCount());
        ImGui::Text("Indices: %d", stats.GetIndexCount());
	    ImGui::ColorEdit4("Square Color Edit", glm::value_ptr(m_SquareColor));

        ImTextureID textureID = m_Framebuffer->GetColorAttachmentRendererID();
        ImGui::Image(textureID, ImVec2{ 1280.0f, 720.0f }, ImVec2{ 0,1 }, ImVec2{ 1,0 });

        ImGui::End();

	    ImGui::End();
    }
    else {
        ImGui::Begin("SandboxTest");
        auto stats = SL::Renderer2D::GetStats();
        ImGui::Text("Renderer2D Stats:");
        ImGui::Text("Draw Calls: %d", stats.DrawCalls);
        ImGui::Text("Quads: %d", stats.QuadCount);
        ImGui::Text("Vertices: %d", stats.GetVertexCount());
        ImGui::Text("Indices: %d", stats.GetIndexCount());
        ImGui::ColorEdit4("Square Color Edit", glm::value_ptr(m_SquareColor));

        ImTextureID textureID = m_Texture->GetRendererID();
        ImGui::Image(textureID, ImVec2{ 256.0f, 256.0f });

        ImGui::End();
    }

}

void Sandbox2D::OnEvent(SoLin::Event& event)
{
	m_CameraController.OnEvent(event);
}
