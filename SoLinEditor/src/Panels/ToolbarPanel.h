#pragma once

#include"SoLin/Renderer/Texture.h"

namespace SoLin{
    enum class SceneState {
        Edit = 0,Play=1
    };

    class ToolbarPanel {
    public:
        ToolbarPanel();
        void OnImGuiRenderer();
        inline SceneState GetSceneState()const { return m_SceneState; }
        inline void SetSceneState(SceneState ss) { m_SceneState = ss; }
    private:
        void OnScenePlay();
        void OnSceneStop();

        // @brief 设置信息弹出窗
        void ImGuiInfoWindow(const std::string& text);
    private:
        SceneState m_SceneState = SceneState::Edit;

        Ref<Texture2D> m_PlayIcon;
        Ref<Texture2D> m_StopIcon;

        bool m_ShowPop = false; //信息弹窗 标志
    };
}
