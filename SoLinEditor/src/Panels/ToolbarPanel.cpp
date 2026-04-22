#include "slpch.h"
#include"ToolbarPanel.h"

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
        m_SceneState = SceneState::Play;
    }
    void ToolbarPanel::OnSceneStop()
    {
        m_SceneState = SceneState::Edit;
    }
}
