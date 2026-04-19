#include "EditorLayer.h"

#include"SoLin/Scene/ScriptableEntity.h"
#include"SoLin/Scene/SceneSerializer.h"
#include"SoLin/Utils/PlatformUtils.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include<imgui/imgui.h>
#include<ImGuizmo.h>

#include"SoLin/Math/Math.h"

namespace SoLin {

    EditorLayer* EditorLayer::s_Instance = nullptr;

    EditorLayer::EditorLayer()
        :Layer("Editor"), m_CameraController(1280.0f / 720.0f, true)
    {
        SL_CORE_ASSERT(!s_Instance, "EditorLayer Instance already exists!(EditorLayer is a Singleton!)");
        s_Instance = this;
    }

    void EditorLayer::OnAttach()
    {
        SL_PROFILE_FUNCTION();

        m_Framebuffer = FrameBuffer::Create(
            { 1280,720,1,{
                FrameBufferAttachmentFormat::RGBA8,
                FrameBufferAttachmentFormat::RED_INTEGER,
                FrameBufferAttachmentFormat::Depth}
            });

        m_Texture = SoLin::Texture2D::Create(SLPATH("assets/textures/千夏02.png"));
        m_TexShelter.push_back(m_resource.getTexture("assets/textures/shelter_m.png"));
        m_TexShelter.push_back(m_resource.getTexture("assets/textures/shelter_n.png"));
        m_TexShelter.push_back(m_resource.getTexture("assets/textures/shelter_e.png"));
        m_TexShelter.push_back(m_resource.getTexture("assets/textures/shelter_w.png"));

        m_ActiveScene = CreateRef<Scene>();
        m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

        m_SquareEntity = m_ActiveScene->CreateEntity("Square");
        m_SquareEntity.AddComponent<SpriteComponent>(m_SquareColor);
        m_BlueSquare = m_ActiveScene->CreateEntity("BlueSquare");
        m_BlueSquare.AddComponent<SpriteComponent>(glm::vec4{ 0.0f, 1.0f, 1.0f, 1.0f });
        m_RedSquare = m_ActiveScene->CreateEntity("RedSquare");
        m_RedSquare.AddComponent<SpriteComponent>(glm::vec4{ 1.0f, 0.0f, 1.0f, 1.0f });

        // 创建主相机实体，传入视口矩阵
        m_CameraEntity = m_ActiveScene->CreateEntity("Main-Camera");
        auto& firstController = m_CameraEntity.AddComponent<CameraComponent>();
        firstController.Primary = true;
        m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<ScriptCameraController>();			//添加本机脚本

        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
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

            m_EditorCamera.SetViewportSize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }
        //Update
        if(m_ViewportFocused)
            m_CameraController.OnUpdate(ts);
        m_EditorCamera.OnUpdate(ts);
        //Render
        {
            SL_PROFILE_SCOPE("RenderCommand Prep");
            Renderer2D::ClearStats();// 每次更新前都要将Stats统计数据清零
            m_Framebuffer->Bind();
            SoLin::RendererCommand::SetClearColor({ 0.1f,0.1f,0.1f,1 });
            SoLin::RendererCommand::Clear();

            // Clear entity ID to -1
            m_Framebuffer->ClearAttachment(1, -1);
        }

        {
            SL_PROFILE_SCOPE("Renderer2D Draw");

#if 1
            /*static float temp = 0.0f;
            temp += ts * 100.0f;

            Renderer2D::BeginScene(m_CameraController.GetCamera());
            Renderer2D::DrawQuad({ 0.0f,0.0f }, { 1.0f, 1.0f }, { 0.8f, 0.2f, 0.3f, 1.0f });
            Renderer2D::DrawQuad({ 1.0f, -1.0f,-0.2 }, { 0.5f, 1.0f }, m_SquareColor);
            Renderer2D::DrawRotatedQuad({ -1.0f, 1.0f }, { 1.0f, 1.0f }, glm::radians(temp), { 0.3f, 0.2f, 0.8f, 1.0f });
            Renderer2D::DrawQuad({ 0.0f,0.0f,-0.1f }, { 2.0f,2.0f }, m_Texture, 10.0f, { 1.0,0.9,0.9,1.0 });
            int i = int(temp / 100)%3;
            Renderer2D::DrawQuad({ 0.0f,0.0f,0.1f }, { 1.0f,1.0f }, m_TexShelter[i], 1.0f, {1.0,1.0,1.0,1.0});
            Renderer2D::EndScene();*/


            //m_ActiveScene->OnUpdate(ts);
            //m_ActiveScene->OnScript(ts);  // 更新本机脚本
            m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);

            // Read Pixels from attachment
            // 获取视口尺寸
            glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
            // 获取鼠标位置
            auto [mX, mY] = ImGui::GetMousePos();
            mX -= m_ViewportBounds[0].x;
            mY -= m_ViewportBounds[0].y;
            // imgui是从左上为起始的，需要进行垂直方向的颠倒，所以要取反的
            mY = viewportSize.y - mY;

            int mouseX = (int)mX;
            int mouseY = (int)mY;
            if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
            {
                int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
                SL_CORE_WARN("Mouse: {0},{1}", mouseX, mouseY);
                // 放在物体上就会显示50，其他视口位置就会显示一个随机数
                SL_CORE_WARN("Pixel data: {0}", pixelData);
            }
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

        ImGuiStyle& style = ImGui::GetStyle();// 获取样式引用
        float minWinSizeX = style.WindowMinSize.x;// 临时存储最小宽度(结束时要恢复)
        float minWinSizeY = style.WindowMinSize.y;
        style.WindowMinSize.x = 370.0f;// 为所创建的MyDockSpace使用的最小宽度
        style.WindowMinSize.y = 180.0f;

        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
        style.WindowMinSize.x = minWinSizeX;// 恢复
        style.WindowMinSize.y = minWinSizeY;

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
                if (ImGui::MenuItem("New","Ctrl + N"))
                    NewScene();
                if (ImGui::MenuItem("Save AS ...", "Ctrl + S"))
                    SaveSceneAs();
                if (ImGui::MenuItem("Open ...","Ctrl + O"))
                    OpenScene();
                if (ImGui::MenuItem("Exit"))
                    SoLin::Application::Get().WindowClose();

                /*ImGui::Separator();
                if (ImGui::MenuItem("Close", NULL, false))
                    dockspaceOpen = false;*/

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        m_SceneHierarchyPanel.OnImGuiRender();

        ImGui::Begin("Stats");
        auto stats = Renderer2D::GetStats();
        ImGui::Text("Renderer2D Stats:");
        ImGui::Text("Draw Calls: %d", stats.DrawCalls);
        ImGui::Text("Quads: %d", stats.QuadCount);
        ImGui::Text("Vertices: %d", stats.GetVertexCount());
        ImGui::Text("Indices: %d", stats.GetIndexCount());


        ImGui::End();//Test

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Viewport");
        // 用于获取 ImGui 中下一个 UI 元素将要放置的位置
        // 也就是整个Imgui窗口的左上角到Viewport左上角的偏移。
        auto viewportOffset = ImGui::GetCursorPos();

        m_ViewportFocused = ImGui::IsWindowFocused();   // 更新聚焦标志
        m_ViewportHovered = ImGui::IsWindowHovered();   // 更新悬浮标志
        // 该Imgui视口挡在了原本窗口之上，但其充当原先窗口来使用，所以聚焦并且鼠标悬浮在上面时
        // 传入F，将原本在imgui的事件阻塞，让原本的窗口去执行
        Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

        ImVec2 panelSize = ImGui::GetContentRegionAvail();  //获取面板大小
        m_ViewportSize = { panelSize.x, panelSize.y };

        ImTextureID textureID = m_Framebuffer->GetColorAttachmentRendererID(0);
        ImGui::Image(textureID, ImVec2{ m_ViewportSize.x,m_ViewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });

        // 确认边界值
        ImVec2 windowSize = ImGui::GetWindowSize(); // 获取整个 ImGui 窗口的总大小
        ImVec2 minBound = ImGui::GetWindowPos();    // 获取整个 ImGui 窗口左上角在显示器上的坐标
        minBound.x += viewportOffset.x;
        minBound.y += viewportOffset.y;
        ImVec2 maxBound = { minBound.x + m_ViewportSize.x, minBound.y + m_ViewportSize.y };
        // 存储边界值
        m_ViewportBounds[0] = { minBound.x, minBound.y };
        m_ViewportBounds[1] = { maxBound.x, maxBound.y };

        // Gizmos
        Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
        if (selectedEntity && m_GizmoType != -1)
        {
            // 设置ImGuizmo的工作环境
            ImGuizmo::SetOrthographic(false);           // 使用透视相机（不是正交）
            ImGuizmo::SetDrawlist();                    // 使用当前ImGui的绘制列表
            ImGuizmo::SetRect(
                ImGui::GetWindowPos().x, ImGui::GetWindowPos().y,
                (float)ImGui::GetWindowWidth(), (float)ImGui::GetWindowHeight()
            );

            // Get camera projection matrix & camera view matrix & transform matrix
            Entity cameraEntity = m_ActiveScene->GetPrimaryCamera();
#if 0       // 以场景相机方式获取
            // Camera Projection
            const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
            const auto& cameraProjection = camera.GetProjection();
            // Camera View
            auto& cameraTransform = cameraEntity.GetComponent<TransformComponent>().GetTransform();
            glm::mat4 cameraView = glm::inverse(cameraTransform);
#endif
#if 1       // 直接从编辑器相机获取(暂时测试)
            const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
            const glm::mat4& cameraView = m_EditorCamera.GetViewMatrix();
#endif
            // Transform
            auto& tc = selectedEntity.GetComponent<TransformComponent>();
            glm::mat4 transform = tc.GetTransform();

            // Snapping     设置吸附
            float snapValue = 0.5f;
            bool snap = Input::IsKeyPressed(SL_KEY_LEFT_CONTROL);
            if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
                snapValue = 10.0f;
            float snapValues[3] = { snapValue, snapValue, snapValue };

            // Manipulate   显示并操作Gizmo
            ImGuizmo::Manipulate(
                glm::value_ptr(cameraView),             // 视图矩阵
                glm::value_ptr(cameraProjection),       // 投影矩阵
                ImGuizmo::OPERATION(m_GizmoType),       // 操作类型：T R S
                ImGuizmo::LOCAL,                        // 本地坐标（相对于物体自身）
                glm::value_ptr(transform),              // 输入输出的变换矩阵
                nullptr,                                // 可选：delta矩阵
                snap ? snapValues : nullptr);           // 吸附值（如果启用）

            // 用户正在拖拽Gizmo
            if (ImGuizmo::IsUsing())
            {
                glm::vec3 translation, rotation, scale;
                Math::DecomposeTransform(transform, translation, rotation, scale);
                glm::vec3 deltaRotation = rotation - tc.Rotation;
                tc.Translation = translation;
                tc.Rotation += deltaRotation;
                tc.Scale = scale;
            }
        }

        ImGui::PopStyleVar();
        ImGui::End();//Viewport


        ImGui::End();//DockSpace Demo

    }

    void EditorLayer::OnEvent(SoLin::Event& event)
    {
        m_CameraController.OnEvent(event);
        m_EditorCamera.OnEvent(event);

        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyPressedEvent>(SOLIN_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
    }

    bool EditorLayer::OnKeyPressed(KeyPressedEvent& event)
    {
        if (event.GetRepeatCount() > 0)
            return false;

        bool ctrl = Input::IsKeyPressed(SL_KEY_LEFT_CONTROL) || Input::IsKeyPressed(SL_KEY_RIGHT_CONTROL);
        bool shift = Input::IsKeyPressed(SL_KEY_LEFT_SHIFT) || Input::IsKeyPressed(SL_KEY_RIGHT_SHIFT);
        switch (event.GetKeyCode()) {
        // Dialog 操作
        case SL_KEY_N:
            if (ctrl)
                NewScene();
            break;
        case SL_KEY_O:
            if (ctrl)
                OpenScene();
            break;
        case SL_KEY_S:
            if (ctrl)
                SaveSceneAs();
            break;

        // Gizmo 操作
        case SL_KEY_Q:
            m_GizmoType = -1;
            break;
        case SL_KEY_W:
            m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
            break;
        case SL_KEY_E:
            m_GizmoType = ImGuizmo::OPERATION::ROTATE;
            break;
        case SL_KEY_R:
            m_GizmoType = ImGuizmo::OPERATION::SCALE;
            break;
        }

    }

    void EditorLayer::NewScene()
    {
        m_ActiveScene = CreateRef<Scene>();
        m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
    }
    void EditorLayer::OpenScene()
    {
        std::string filepath = FileDialogs::OpenFile("SoLin Scene(*.yaml)\0 *.yaml\0All Files (*.*)\0*.*\0\0");
        // 如果文件路径不为空就新创建场景再做反序列化读取数据
        if (!filepath.empty()) {
            m_ActiveScene = CreateRef<Scene>();
            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_SceneHierarchyPanel.SetContext(m_ActiveScene);

            SceneSerializer serializer(m_ActiveScene);
            serializer.Deserialize(filepath);
        }
    }
    void EditorLayer::SaveSceneAs()
    {
        std::string filepath = FileDialogs::SaveFile("SoLin Scene()(*.yaml)\0 *.yaml\0");
        // 如果文件路径不为空就做序列化存储
        if (!filepath.empty()) {
            SceneSerializer deserializer(m_ActiveScene);
            deserializer.Serialize(filepath);
        }
    }
}
