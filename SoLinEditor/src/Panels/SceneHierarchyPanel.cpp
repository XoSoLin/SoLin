#include "slpch.h"
#include"SceneHierarchyPanel.h"
#include"SoLin/Scene/Component.h"
#include "../EditorLayer.h"

#include<imgui/imgui.h>
#include <imgui/imgui_internal.h>
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
        // 场景层级结构---------------------------------------------------------
        ImGui::Begin("Scene Hierarchy");
        m_Context->m_Registry.view<entt::entity>().each(
            [&](auto entityID) {
                Entity entity{ entityID,m_Context.get() };
                // 对场景中每一个实体进行树状结点绘制
                DrawEntityNode(entity);
            }
        );
        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
            m_SelectionContext = {};

        // 右击出现窗口菜单
        if (ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems)) {
            // 创建空实体
            if (ImGui::MenuItem("Create Empty Entity"))
                m_Context->CreateEntity("Empty Entity");
            ImGui::EndPopup();
        }

        ImGui::End();//Scene Hierarchy 场景层级结构

        // 选中实体属性面板---------------------------------------------------
        ImGui::Begin("Properties");
        if (m_SelectionContext)
        {
            DrawComponents(m_SelectionContext);

            if (ImGui::Button("AddComponent"))
                ImGui::OpenPopup("AddComponentMenu");

            if (ImGui::BeginPopup("AddComponentMenu")) {
                if (ImGui::MenuItem("CmaeraComponent")) {
                    m_SelectionContext.AddComponent<CameraComponent>();
                    ImGui::CloseCurrentPopup();
                }
                if (ImGui::MenuItem("SpriteRendererComponent")) {
                    m_SelectionContext.AddComponent<SpriteComponent>();
                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }

        }

        ImGui::End();//Properties 选中实体属性面板
    }

    void SceneHierarchyPanel::DrawEntityNode(Entity& entity)
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

        bool entityDeleted = false;// 实体删除标志
        // 在“上一个已注册的控件”上点击鼠标右键时，打开一个弹出上下文菜单。
        if (ImGui::BeginPopupContextItem()) {
            // 在弹出菜单中创建一个可点击的菜单项。
            if (ImGui::MenuItem("Delete Entity"))
                entityDeleted = true;

            ImGui::EndPopup();
        }

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

        // 如果删除标志为真，则移除当前选定的实体
        if (entityDeleted) {
            m_Context->DestoryEntity(entity);
            if (entity == m_SelectionContext)
                m_SelectionContext = {};
        }

    }

    void SceneHierarchyPanel::DrawComponents(Entity& entity)
    {
//------------------------------TagComponent--------------------------------
        if (entity.HasComponent<TagComponent>()) {
            auto& tag = entity.GetComponent<TagComponent>().Tag;

            if (ImGui::TreeNodeEx((void*)typeid(TagComponent).hash_code(),
                ImGuiTreeNodeFlags_DefaultOpen,"Tag")) {
                // 分为两列
                ImGui::Columns(2);
                ImGui::SetColumnWidth(0, 100.0f);
                ImGui::Text("Tagname");

                // 移动到第二列
                ImGui::NextColumn();
                // 创建缓冲区并清0，然后传入tag内容
                char buffer[256];
                memset(buffer, 0, sizeof(buffer));
                strcpy_s(buffer, sizeof(buffer), tag.c_str());
                // 创建一个标签为 "Tag" 的输入框，显示 / 编辑 buffer 的内容。
                // ##表示隐藏
                if (ImGui::InputText("##Tag", buffer, sizeof(buffer))) {
                    tag = std::string(buffer);
                }
                // 总结就是将tag数据传输给buffer，然后再由buffer实时修改tag

                ImGui::Columns(1);

                ImGui::TreePop();
            }

        }


//------------------------------TransformComponent--------------------------------
        if (entity.HasComponent<TransformComponent>()) {
            if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(),        // 使用类型哈希码作为唯一 ID，确保同一类型组件只创建一次节点
                ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
            {
                auto& tc = entity.GetComponent<TransformComponent>();

                DrawVec3Controller("Translation", tc.Translation);

                glm::vec3 rotation = glm::degrees(tc.Rotation);
                DrawVec3Controller("Rotation", rotation);
                tc.Rotation = glm::radians(rotation);

                DrawVec3Controller("Scale", tc.Scale, 1.0f);
                ImGui::TreePop();//Transform
            }
        }

//------------------------------CameraComponent--------------------------------
        if(entity.HasComponent<CameraComponent>())
        {
            bool open = ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(),
                ImGuiTreeNodeFlags_DefaultOpen, "Camera");

            // 按钮必要数据
            float buttonWidth = ImGui::CalcTextSize("+").x + GImGui->Style.FramePadding.x * 2.0f;
            float buttonHeight = ImGui::CalcTextSize("+").y + GImGui->Style.FramePadding.y * 2.0f;

            // 创建一个“+”按钮
            // 将下一个控件放置在与窗口右边缘保持固定距离（25像素）的位置。
            ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);
            if (ImGui::Button("+", { buttonWidth,buttonHeight })) {
                // 打开一个名为 "ComponentSettings" 的弹出窗口/上下文菜单。
                ImGui::OpenPopup("ComponentSettings");
            }

            bool deleted = false;   // 移除组件标志
            // 如果组件设置激活，出现一个菜单，可以移除组件
            if (ImGui::BeginPopup("ComponentSettings")) {
                if (ImGui::MenuItem("Remove component"))
                    deleted = true;

                ImGui::EndPopup();
            }

            // 如果相机组件结点存在，就画
            if (open)
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
                    if (ImGui::DragFloat("Vertical FOV", &verticalFov, 1.0f, 30.0f, 120.0f))
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

            // 移除标志激活就移除相机组件
            if (deleted) {
                entity.RemoveComponent<CameraComponent>();
            }
        }

//------------------------------SpriteComponent--------------------------------
        if (entity.HasComponent<SpriteComponent>()) {

            bool open = ImGui::TreeNodeEx((void*)typeid(SpriteComponent).hash_code(),
                ImGuiTreeNodeFlags_DefaultOpen, "Sprite Renderer");

            // 按钮必要数据
            float buttonWidth = ImGui::CalcTextSize("+").x + GImGui->Style.FramePadding.x * 2.0f;
            float buttonHeight = ImGui::CalcTextSize("+").y + GImGui->Style.FramePadding.y * 2.0f;

            // 创建一个“+”按钮
            // 将下一个控件放置在与窗口右边缘保持固定距离（25像素）的位置。
            ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);
            if (ImGui::Button("+", { buttonWidth,buttonHeight })) {
                // 打开一个名为 "ComponentSettings" 的弹出窗口/上下文菜单。
                ImGui::OpenPopup("ComponentSettings");
            }
            
            bool deleted = false;   // 移除组件标志
            // 如果组件设置激活，出现一个菜单，可以移除组件
            if (ImGui::BeginPopup("ComponentSettings")) {
                if (ImGui::MenuItem("Remove component"))
                    deleted = true;

                ImGui::EndPopup();
            }

            // 如果精灵组件结点存在，就画出来
            if(open) {
                auto& controller = entity.GetComponent<SpriteComponent>();
                ImGui::ColorEdit4("Color", glm::value_ptr(controller.Color));
                ImGui::TreePop();
            }

            // 如果移除标志激活，就移除精灵组件
            if (deleted) {
                entity.RemoveComponent<SpriteComponent>();
            }
        }

    }

    void SceneHierarchyPanel::DrawVec3Controller(const std::string& label, glm::vec3& values, float resetValue, float columnWidth)
    {
        ImGui::PushID(label.c_str());

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label.c_str());

        ImGui::NextColumn();
        {
            float lineHeight = ImGui::GetFontSize() + GImGui->Style.FramePadding.y * 2.0f;
            ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

            ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0,0 });
            // --------- x -----------
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.6f, 0.1f, 0.0f, 1.0f });
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.1f, 0.0f, 1.0f });
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.6f, 0.1f, 0.0f, 1.0f });
            if (ImGui::Button("X", buttonSize))
            {
                values.x = resetValue;
            }
            ImGui::PopStyleColor(3);

            ImGui::SameLine();
            ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
            ImGui::PopItemWidth();
            // ---------- y -----------
            ImGui::SameLine();
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f, 0.6f, 0.1f, 1.0f });
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.0f, 0.9f, 0.1f, 1.0f });
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.0f, 0.6f, 0.1f, 1.0f });
            if (ImGui::Button("Y", buttonSize))
            {
                values.y = resetValue;
            }
            ImGui::PopStyleColor(3);

            ImGui::SameLine();
            ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
            ImGui::PopItemWidth();
            // ----------- z ------------
            ImGui::SameLine();
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.0f, 0.6f, 1.0f });
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.1f, 0.0f, 0.9f, 1.0f });
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.0f, 0.6f, 1.0f });
            if (ImGui::Button("Z", buttonSize))
            {
                values.z = resetValue;
            }
            ImGui::PopStyleColor(3);

            ImGui::SameLine();
            ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
            ImGui::PopItemWidth();

            ImGui::PopStyleVar();
        }

        ImGui::Columns(1);

        ImGui::PopID();
    }
}
