#pragma once

#include "SoLin.h"
#include"Panels/SceneHierarchyPanel.h"

namespace SoLin {

    class EditorLayer :public Layer {
    public:
        EditorLayer();
	    virtual ~EditorLayer() = default;

	    virtual void OnAttach() override;
	    virtual void OnDetach()override;

	    virtual void OnUpdate(Timestep ts) override;
	    virtual void OnImGuiRender()override;
	    virtual void OnEvent(Event& event)override;
    private:
	    Ref<Texture2D> m_Texture;
        std::vector<Ref<Texture2D>> m_TexShelter;

        Ref<SoLin::FrameBuffer> m_Framebuffer;

        Ref<Scene> m_ActiveScene;
        Entity m_SquareEntity;

        Entity m_CameraEntity, m_SecondCamera;// 相机实体

        bool m_PrimaryCamera = true;

        bool m_ViewportFocused = false;     // 视口聚焦标志
        bool m_ViewportHovered = false;     // 鼠标悬浮在视口标志
        glm::vec2 m_ViewportSize = { 0.0f,0.0f };

	    OrthoGraphicCameraController m_CameraController;

	    glm::vec4 m_SquareColor = { 0.5412f, 0.1686f, 0.8863f, 1.0f };

        //Panels
        SceneHierarchyPanel m_HierarchyPanel;
    };
}
