#include "slpch.h"
#include"ToolbarPanel.h"

#include"../EditorLayer.h"

#include<imgui/imgui.h>

namespace SoLin {
    ToolbarPanel::ToolbarPanel()
    {
        m_PlayIcon = Texture2D::Create("Resources/Icons/ContentBrowser/PlayButton3.png");
        m_StopIcon = Texture2D::Create("Resources/Icons/ContentBrowser/PauseButton3.png");
    }
    void ToolbarPanel::OnImGuiRenderer()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        auto& colors = ImGui::GetStyle().Colors;
        const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
        const auto& buttonActive = colors[ImGuiCol_ButtonActive];
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

        ImGui::Begin("##Toolbar panel",
            nullptr,                            // 不提供开关控制，窗口无法被用户关闭
            ImGuiWindowFlags_NoDecoration |     // 无标题栏、不可调整大小、不可折叠
            ImGuiWindowFlags_NoScrollbar |      // 不显示滚动条
            ImGuiWindowFlags_NoScrollWithMouse  // 禁用鼠标滚轮滚动窗口内容
        );

        float size = ImGui::GetWindowHeight() - 4.0f;
        Ref<Texture2D> icon = (m_SceneState == SceneState::Edit ? m_PlayIcon : m_StopIcon);

        ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

        // 状态切换按钮
        if (ImGui::ImageButton(icon->GetPath().string().c_str(), (ImTextureID)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1)))
        {
            if (m_SceneState == SceneState::Edit)
                OnScenePlay();
            else if (m_SceneState == SceneState::Play)
                OnSceneStop();
        }

        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(3);
        ImGui::End();
    }

    void ToolbarPanel::OnScenePlay()
    {
        // 从编辑器层获取必要的 指针
        Ref<Scene>& activeScene = EditorLayer::Get().m_ActiveScene;
        Ref<Scene>& editorScene = EditorLayer::Get().m_EditorScene;
        SceneHierarchyPanel& sceneHierarchyPanel = EditorLayer::Get().m_SceneHierarchyPanel;

        // 状态更新并 复制原场景 作为 激活场景
        m_SceneState = SceneState::Play;
        activeScene = Scene::Copy(editorScene);
        activeScene->OnRuntimeStart();
        sceneHierarchyPanel.SetContext(activeScene);
    }

    void ToolbarPanel::OnSceneStop()
    {
        // 从编辑器层获取必要的 指针
        Ref<Scene>& activeScene = EditorLayer::Get().m_ActiveScene;
        Ref<Scene>& editorScene = EditorLayer::Get().m_EditorScene;
        SceneHierarchyPanel& sceneHierarchyPanel = EditorLayer::Get().m_SceneHierarchyPanel;

        // 状态更新并 舍弃激活场景 使激活场景指向编辑器场景
        m_SceneState = SceneState::Edit;
        activeScene->OnRuntimeStop();   // 要先执行 暂停时可能会影响一些指针(未来可能会有未定义风险 目前没有)
        activeScene = editorScene;
        sceneHierarchyPanel.SetContext(activeScene);
    }
}
