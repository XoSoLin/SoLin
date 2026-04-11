#pragma once

#include "SoLin.h"
#include"Panels/SceneHierarchyPanel.h"

namespace SoLin {

    // @brief 编辑器 层
    class EditorLayer :public Layer {
    public:
        EditorLayer();
	    virtual ~EditorLayer() = default;
        // 使用 * 对指针类型的变量 s_Instance 进行解引用，返回指针变量所指向对象的引用
        static EditorLayer& Get() { return *s_Instance; }

	    virtual void OnAttach() override;
	    virtual void OnDetach()override;

	    virtual void OnUpdate(Timestep ts) override;
	    virtual void OnImGuiRender()override;
	    virtual void OnEvent(Event& event)override;

    public:
        glm::vec2 GetImGuiViewportSize() { return m_ViewportSize; };
    private:
        // Instance 单例
        static EditorLayer* s_Instance;

	    Ref<Texture2D> m_Texture;
        std::vector<Ref<Texture2D>> m_TexShelter;

        Ref<SoLin::FrameBuffer> m_Framebuffer;

        Ref<Scene> m_ActiveScene;
        Entity m_SquareEntity;
        Entity m_BlueSquare, m_RedSquare;

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
