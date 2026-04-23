#pragma once

#include "SoLin.h"
#include"Panels/SceneHierarchyPanel.h"
#include"Panels/ContentBrowserPanels.h"
#include"Panels/ToolbarPanel.h"

#include"SoLin/Renderer/EditorCamera.h"
#include"SoLin/Scene/Animation/Animation.h"

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

        // @brief 复制实体 场景操作
        void OnDuplicateEntity();

        // @brief 新建场景 选项回调
        void NewScene();
        // @brief 打开场景 选项回调
        void OpenScene();
        void OpenScene(const std::filesystem::path& path);
        // @brief 保存场景为... 选项回调
        void SaveSceneAs();
        // @brief 保存场景 选项回调
        void SaveScene();
    private:
        // @brief 置空 存放原场景信息的实体指针(更换新场景后使用 否则有未定义行为风险)
        void ReSrcScenePtr();
    private:
        // Instance 单例
        static EditorLayer* s_Instance;
        // 资源
        ResourceManager m_resource;

        // 默认演示场景所需
	    Ref<Texture2D> m_Texture;
        std::vector<Ref<Texture2D>> m_TexShelter;
        Ref<Animation> m_Animation,m_Animation2;
        Entity m_CameraEntity;          // 相机实体

        // 编辑器层 缓冲区
        Ref<SoLin::FrameBuffer> m_Framebuffer;
        // 编辑器层 场景指针
        Ref<Scene> m_ActiveScene;       // 激活场景  编辑时 就是编辑时场景; 运行时 是复制而来的全新场景
        Ref<Scene> m_EditorScene;       // 编辑时场景
        // 实体信息
        Entity m_HoveredEntity;         // 鼠标悬浮位置实体
        Entity m_UsingEntity;           // 使用中的实体


        // Bool标志
        bool m_PrimaryCamera = true;    // 主相机标志
        bool m_ViewportFocused = false;     // 视口聚焦标志
        bool m_ViewportHovered = false;     // 鼠标悬浮在视口标志

        // ViewPort
        glm::vec2 m_ViewportSize = { 0.0f,0.0f };// 视口尺寸
        EditorCamera m_EditorCamera;    // 编辑器相机

        // 层相机控制器 目前似乎没啥用了
	    OrthoGraphicCameraController m_CameraController;

        //Panels
        SceneHierarchyPanel m_SceneHierarchyPanel;  // 场景层次面板
        ContentBrowserPanel m_ContentBrowserPanel;  // 内容浏览器面板
        ToolbarPanel m_ToolbarPanel;                // 工具栏面板

        // Bounds
        glm::vec2 m_ViewportBounds[2];  // 视口边界

        // Gizmo
        int m_GizmoType = -1;       // Gizmo状态标志

        // Path
        std::filesystem::path m_ScenePath;  // 编辑中的场景路径

    private:
        friend class ToolbarPanel;
    };
}
