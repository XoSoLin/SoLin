#include "EditorLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include<imgui/imgui.h>

namespace SoLin {

    EditorLayer::EditorLayer()
        :Layer("Editor"), m_CameraController(1280.0f / 720.0f, true)
    {
    }

    void EditorLayer::OnAttach()
    {
        SL_PROFILE_FUNCTION();

        m_Framebuffer = FrameBuffer::Create({ 1280,720 });

        m_Texture = SoLin::Texture2D::Create(SLPATH("assets/textures/千夏02.png"));
        m_TexShelter.push_back(Texture2D::Create(SLPATH("assets/textures/shelter_m.png")));
        m_TexShelter.push_back(Texture2D::Create(SLPATH("assets/textures/shelter_n.png")));
        m_TexShelter.push_back(Texture2D::Create(SLPATH("assets/textures/shelter_e.png")));
        m_TexShelter.push_back(Texture2D::Create(SLPATH("assets/textures/shelter_w.png")));

        m_ActiveScene = CreateRef<Scene>();
        m_SquareEntity = m_ActiveScene->CreateEntity("Square");
        m_SquareEntity.AddComponent<SpriteComponent>(m_SquareColor);

        // 创建主相机实体，传入视口矩阵
        m_CameraEntity = m_ActiveScene->CreateEntity("Main-Camera");
        auto& firstController = m_CameraEntity.AddComponent<CameraComponent>();
        firstController.Primary = true;

        // 创建普通相机实体，传入视口矩阵
        m_SecondCamera = m_ActiveScene->CreateEntity("Clip-Camera");
        auto& secondController = m_SecondCamera.AddComponent<CameraComponent>();
        secondController.Primary = false;
    }

    void EditorLayer::OnDetach()
    {
        SL_PROFILE_FUNCTION();

    }

    void EditorLayer::OnUpdate(SoLin::Timestep ts)
    {
        SL_PROFILE_FUNCTION();

        // 提前处理ReSize,否则Framebuffer尺寸不符合会白屏闪烁
        if ((m_ViewportSize.x != m_Framebuffer->GetSpecification().Width || m_ViewportSize.y != m_Framebuffer->GetSpecification().Height) && m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f)
        {
            m_Framebuffer->ReSize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_CameraController.ReSize(m_ViewportSize.x, m_ViewportSize.y);

            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }
        //Update
        if(m_ViewportFocused)
            m_CameraController.OnUpdate(ts);
        //Render
        {
            SL_PROFILE_SCOPE("RenderCommand Prep");
            Renderer2D::ClearStats();// 每次更新前都要将Stats统计数据清零
            m_Framebuffer->Bind();
            SoLin::RendererCommand::SetClearColor({ 0.1f,0.1f,0.1f,1 });
            SoLin::RendererCommand::Clear();
        }

        {
            SL_PROFILE_SCOPE("Renderer2D Draw");

#if 1
            static float temp = 0.0f;
            temp += ts * 100.0f;

            /*Renderer2D::BeginScene(m_CameraController.GetCamera());
            Renderer2D::DrawQuad({ 0.0f,0.0f }, { 1.0f, 1.0f }, { 0.8f, 0.2f, 0.3f, 1.0f });
            Renderer2D::DrawQuad({ 1.0f, -1.0f,-0.2 }, { 0.5f, 1.0f }, m_SquareColor);
            Renderer2D::DrawRotatedQuad({ -1.0f, 1.0f }, { 1.0f, 1.0f }, glm::radians(temp), { 0.3f, 0.2f, 0.8f, 1.0f });
            Renderer2D::DrawQuad({ 0.0f,0.0f,-0.1f }, { 2.0f,2.0f }, m_Texture, 10.0f, { 1.0,0.9,0.9,1.0 });
            int i = int(temp / 100)%3;
            Renderer2D::DrawQuad({ 0.0f,0.0f,0.1f }, { 1.0f,1.0f }, m_TexShelter[i], 1.0f, {1.0,1.0,1.0,1.0});
            Renderer2D::EndScene();*/

            //Renderer2D::BeginScene(m_CameraController.GetCamera());
            /*for (float y = -5.0f; y < 5.0f; y += 0.5f)
            {
                for (float x = -5.0f; x < 5.0f; x += 0.5f)
                {
                    glm::vec4 color = { 0.0f ,(x + 5.0f) / 10.0f, (y + 5.0f) / 10.0f, 0.7f };
                    Renderer2D::DrawQuad({ x,y }, { 0.45f, 0.45f }, color);
                }
            }*/
            m_ActiveScene->OnUpdate(ts);
            //Renderer2D::EndScene();
#endif
            m_Framebuffer->UnBind();
        }
    }

    void EditorLayer::OnImGuiRender()
    {
        SL_PROFILE_FUNCTION();

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

        ImGui::Begin("Test");
        auto stats = Renderer2D::GetStats();
        ImGui::Text("Renderer2D Stats:");
        ImGui::Text("Draw Calls: %d", stats.DrawCalls);
        ImGui::Text("Quads: %d", stats.QuadCount);
        ImGui::Text("Vertices: %d", stats.GetVertexCount());
        ImGui::Text("Indices: %d", stats.GetIndexCount());
        //if (m_SquareEntity.HasComponent<TagComponent>()) {
        if (m_SquareEntity) {// 重载了bool类型转换
            ImGui::Separator();// 线
            auto& tag = m_SquareEntity.GetComponent<TagComponent>().Tag;
            ImGui::Text("%s", tag.c_str());

            auto& SquareColor = m_SquareEntity.GetComponent<SpriteComponent>().Color;
            ImGui::ColorEdit4("Square Color Edit", glm::value_ptr(SquareColor));
            ImGui::Separator();// 线
        }

        if (ImGui::Checkbox("World space Camera", &m_PrimaryCamera))
        {
            m_CameraEntity.GetComponent<CameraComponent>().Primary = m_PrimaryCamera;
            m_SecondCamera.GetComponent<CameraComponent>().Primary = !m_PrimaryCamera;
        }
        // 变换矩阵会以四元数的方式存储位置信息，在索引3处就相当于相机的位置
        m_PrimaryCamera == true ?
            ImGui::DragFloat3("Camera Transform", glm::value_ptr(m_CameraEntity.GetComponent<TransformComponent>().Transform[3]))
            : ImGui::DragFloat3("Camera Transform", glm::value_ptr(m_SecondCamera.GetComponent<TransformComponent>().Transform[3]));

        auto& camera = m_SecondCamera.GetComponent<CameraComponent>().Camera;
        float orthoSize = camera.GetOrthographicSize();
        if (ImGui::DragFloat("Second Camera Ortho Size", &orthoSize))
            camera.SetOrthographicSize(orthoSize);


        ImGui::End();//Test

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Viewport");

        m_ViewportFocused = ImGui::IsWindowFocused();   // 更新聚焦标志
        m_ViewportHovered = ImGui::IsWindowHovered();   // 更新悬浮标志
        // 该Imgui视口挡在了原本窗口之上，但其充当原先窗口来使用，所以聚焦并且鼠标悬浮在上面时
        // 传入F，将原本在imgui的事件阻塞，让原本的窗口去执行
        Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

        ImVec2 panelSize = ImGui::GetContentRegionAvail();  //获取面板大小
        m_ViewportSize = { panelSize.x, panelSize.y };

        ImTextureID textureID = m_Framebuffer->GetColorAttachmentRendererID();
        ImGui::Image(textureID, ImVec2{ m_ViewportSize.x,m_ViewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });

        ImGui::PopStyleVar();
        ImGui::End();//Viewport


        ImGui::End();//DockSpace Demo

    }

    void EditorLayer::OnEvent(SoLin::Event& event)
    {
        SL_CORE_INFO("{0}",m_DebugName);//测试层对事件的处理
        m_CameraController.OnEvent(event);
    }
}
