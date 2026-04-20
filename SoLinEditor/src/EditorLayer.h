#pragma once

#include "SoLin.h"
#include"Panels/SceneHierarchyPanel.h"
#include"Panels/ContentBrowserPanels.h"

#include"SoLin/Renderer/EditorCamera.h"

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

        bool OnKeyPressed(KeyPressedEvent& event);
        bool OnMouseButtonPressed(MouseButtonPressedEvent& event);
    public:
        // @brief 获取ImGui视口尺寸
        glm::vec2 GetImGuiViewportSize() { return m_ViewportSize; };

        // @brief 新建场景 选项回调
        void NewScene();
        // @brief 打开场景 选项回调
        void OpenScene();
        // @breif 保存场景为... 选项回调
        void SaveSceneAs();
    private:
        // Instance 单例
        static EditorLayer* s_Instance;

	    Ref<Texture2D> m_Texture;
        std::vector<Ref<Texture2D>> m_TexShelter;
        ResourceManager m_resource;

        Ref<SoLin::FrameBuffer> m_Framebuffer;

        Ref<Scene> m_ActiveScene;
        Entity m_SquareEntity;
        Entity m_BlueSquare, m_RedSquare;

        Entity m_HoveredEntity; // 鼠标悬浮位置实体
        Entity m_UsingEntity;// 使用中的实体

        Entity m_CameraEntity, m_SecondCamera;// 相机实体

        bool m_PrimaryCamera = true;

        bool m_ViewportFocused = false;     // 视口聚焦标志
        bool m_ViewportHovered = false;     // 鼠标悬浮在视口标志
        glm::vec2 m_ViewportSize = { 0.0f,0.0f };

	    OrthoGraphicCameraController m_CameraController;

	    glm::vec4 m_SquareColor = { 0.5412f, 0.1686f, 0.8863f, 1.0f };

        EditorCamera m_EditorCamera;    // 编辑器相机
        //Panels
        SceneHierarchyPanel m_SceneHierarchyPanel; // 场景层次面板
        ContentBrowserPanel m_ContentBrowserPanel; // 内容浏览器面板

        // Bounds
        glm::vec2 m_ViewportBounds[2];  // 视口边界

        // Gizmo
        int m_GizmoType = -1;       // Gizmo标志
    };
}
