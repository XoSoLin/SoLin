#pragma once

#include"SoLin/Scene/Scene.h"
#include"SoLin/Scene/Entity.h"

namespace SoLin {

    // 场景 的 层次结构 面板
    class SceneHierarchyPanel {
    public:
        SceneHierarchyPanel() = default;
        SceneHierarchyPanel(const Ref<Scene>& scene);

        // @brief 设置场景上下文
        void SetContext(const Ref<Scene>& scene);

        // @brief 渲染ImGui
        void OnImGuiRender();
    private:
        // @brief 画实体结点
        void DrawEntityNode(Entity entity);
        // @brief 画所含组件
        void DrawComponents(Entity entity);
    private:
        // @brief 场景上下文
        Ref<Scene> m_Context;
        // @brief 选择实体上下文
        Entity m_SelectionContext;
    };
}
