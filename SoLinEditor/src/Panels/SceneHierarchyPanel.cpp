#include "slpch.h"
#include"SceneHierarchyPanel.h"
#include"SoLin/Scene/Component.h"

#include<imgui/imgui.h>

namespace SoLin {
    SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
    {
        SetContext(scene);
    }
    void SceneHierarchyPanel::SetContext(const Ref<Scene>& scene)
    {
        m_Context = scene;
    }
    void SceneHierarchyPanel::OnImGuiRender()
    {
        ImGui::Begin("Scene Hierarchy");// 场景层级结构
        m_Context->m_Registry.view<entt::entity>().each(
            [&](auto entityID) {
                Entity entity{ entityID,m_Context.get() };
                // 对场景中每一个实体进行树状结点绘制
                DrawEntityNode(entity);
            }
        );

        ImGui::End();
    }
    void SceneHierarchyPanel::DrawEntityNode(Entity entity)
    {
        auto& tag = entity.GetComponent<TagComponent>().Tag;

        //Draw first TreeNode
        ImGuiTreeNodeFlags flags =
            ImGuiTreeNodeFlags_OpenOnArrow |    // 添加箭头
            ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0);// 如果当前entity是被选中实体，则添加高亮
        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
        // 检测上一个控件是否被点击，被点击就设置为选中状态
        if (ImGui::IsItemClicked)
            m_SelectionContext = entity;

        // 如果父节点是展开状态
        if (opened) {
            // Draw nested TreeNode
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
            // (void*)666999是一个占位符
            bool opened = ImGui::TreeNodeEx((void*)666999, flags, tag.c_str());
            if (opened)     // TreeNodeEx 开始一个节点，TreePop 结束一个节点
                ImGui::TreePop();

            ImGui::TreePop();
        }

    }
}
