#include "slpch.h"
#include"SceneHierarchyPanel.h"
#include"SoLin/Scene/Component.h"
#include "../EditorLayer.h"

#include<imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

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
        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
            m_SelectionContext = {};
        ImGui::End();//Scene Hierarchy

        ImGui::Begin("Properties");
        if (m_SelectionContext)
            DrawComponents(m_SelectionContext);

        ImGui::End();//Properties
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
        if (ImGui::IsItemClicked())
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
    void SceneHierarchyPanel::DrawComponents(Entity entity)
    {
        if (entity.HasComponent<TagComponent>()) {
            auto& tag = entity.GetComponent<TagComponent>().Tag;

            // 创建缓冲区并清0，然后传入tag内容
            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strcpy_s(buffer, sizeof(buffer), tag.c_str());
            // 创建一个标签为 "Tag" 的输入框，显示 / 编辑 buffer 的内容。
            if (ImGui::InputText("Tag", buffer, sizeof(buffer))) {
                tag = std::string(buffer);
            }
            // 总结就是将tag数据传输给buffer，然后再由buffer实时修改tag
        }

        if (entity.HasComponent<TransformComponent>()) {
            if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(),        // 使用类型哈希码作为唯一 ID，确保同一类型组件只创建一次节点
                ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
            {
                auto& transform = entity.GetComponent<TransformComponent>().Transform;
                // 创建三个拖动输入框（X, Y, Z），用于编辑位置信息
                ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);
                ImGui::TreePop();//Transform
            }
        }

        if(entity.HasComponent<CameraComponent>())
        {
            if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(),
                ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
            {
                // 获取必要数据
                auto& cameraComponent = entity.GetComponent<CameraComponent>();
                auto& camera = cameraComponent.Camera;
                bool& primary = cameraComponent.Primary;
                bool& fixedAspectRatio = cameraComponent.FixedAspectRatio;

                ImGui::Checkbox("Primary", &primary);

                const char* projectionType[] = { "Perspective","Orthographic" };
                const char* currentProjectionType = projectionType[(int)camera.GetProjectionType()];
                if (ImGui::BeginCombo("Projection", currentProjectionType)) {
                    for (int i = 0;i < 2;i++) {
                        bool isSelected = currentProjectionType == projectionType[i];
                        if (ImGui::Selectable(projectionType[i], isSelected)) {
                            currentProjectionType = projectionType[i];
                            cameraComponent.Camera.SetProjectionType((SceneCamera::ProjectionType)i);
                            glm::vec2 viewportSize = EditorLayer::Get().GetImGuiViewportSize();
                            m_Context->OnViewportResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
                        }
                        if (isSelected)
                            //用于更新焦点（焦点不同于高亮显示）
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();//Projection
                }

                // -------- Draw Perspective Camera Controller --------
                if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
                {
                    float verticalFov = glm::degrees(camera.GetPerspectiveVerticalFOV());
                    if (ImGui::DragFloat("Vertical FOV", &verticalFov))	
                        camera.SetPerspectiveVerticalFOV(glm::radians(verticalFov));

                    float perspectiveNear = camera.GetPerspectiveNearClip();
                    if (ImGui::DragFloat("Near", &perspectiveNear))
                        camera.SetPerspectiveNearClip(perspectiveNear);

                    float perspectiveFar = camera.GetPerspectiveFarClip();
                    if (ImGui::DragFloat("Far", &perspectiveFar))
                        camera.SetPerspectiveFarClip(perspectiveFar);
                }

                // -------- Draw Orthographic Camera Controller --------
                if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
                {
                    ImGui::Checkbox("Fixed Aspect Ratio", &fixedAspectRatio);

                    float orthoSize = camera.GetOrthographicSize();
                    if (ImGui::DragFloat("Size", &orthoSize))
                        camera.SetOrthographicSize(orthoSize);

                    float orthoNear = camera.GetOrthographicNearClip();
                    if (ImGui::DragFloat("Near", &orthoNear))
                        camera.SetOrthographicNearClip(orthoNear);

                    float orthoFar = camera.GetOrthographicFarClip();
                    if (ImGui::DragFloat("Far", &orthoFar))
                        camera.SetOrthographicFarClip(orthoFar);
                }
                ImGui::TreePop();//Camera
            }
        }

    }
}
