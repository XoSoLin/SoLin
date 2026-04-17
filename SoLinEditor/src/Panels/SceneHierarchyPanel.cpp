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
        m_SelectionContext = {};
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
        }

        ImGui::End();//Properties 选中实体属性面板
    }

    void SceneHierarchyPanel::DrawEntityNode(Entity& entity)
    {
        auto& tag = entity.GetComponent<TagComponent>().Tag;

        //Draw first TreeNode
        ImGuiTreeNodeFlags flags =
            ImGuiTreeNodeFlags_OpenOnArrow |    // 添加箭头
            ImGuiTreeNodeFlags_SpanAvailWidth | // 将点击响应区域向右延伸，即使点击文字右侧的空白也可响应
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
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
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


                ImGui::SameLine();          // 下一个控件在同一行
                ImGui::PushItemWidth(-1);   // 设置width，-1表示当前可用区域的剩余宽度
                {
                    // AddComponent 按钮
                    if (ImGui::Button("AddComponent"))
                        ImGui::OpenPopup("AddComponentMenu");

                    // AddComponentMenu 弹出框
                    if (ImGui::BeginPopup("AddComponentMenu")) {

                        if (ImGui::MenuItem("CameraComponent")) {
                            m_SelectionContext.AddComponent<CameraComponent>();
                            ImGui::CloseCurrentPopup();
                        }
                        if (ImGui::MenuItem("SpriteComponent")) {
                            m_SelectionContext.AddComponent<SpriteComponent>();
                            ImGui::CloseCurrentPopup();
                        }
                        ImGui::EndPopup(); //AddComponentMenu
                    }
                }
                ImGui::PopItemWidth();

        }


//------------------------------TransformComponent--------------------------------
        DrawComponent<TransformComponent>("Transform", entity, [](auto& tc) {

                DrawVec3Controller("Translation", tc.Translation);

                glm::vec3 rotation = glm::degrees(tc.Rotation);
                DrawVec3Controller("Rotation", rotation);
                tc.Rotation = glm::radians(rotation);

                DrawVec3Controller("Scale", tc.Scale, 1.0f);
        });

//------------------------------CameraComponent--------------------------------
        DrawComponent<CameraComponent>("Camera", entity, [this](auto& cc) {

            auto& camera = cc.Camera;
            bool& primary = cc.Primary;
            bool& fixedAspectRatio = cc.FixedAspectRatio;

            ImGui::Checkbox("Primary", &primary);

            const char* projectionType[] = { "Perspective","Orthographic" };
            const char* currentProjectionType = projectionType[(int)camera.GetProjectionType()];
            if (ImGui::BeginCombo("Projection", currentProjectionType)) {
                for (int i = 0;i < 2;i++) {
                    bool isSelected = currentProjectionType == projectionType[i];
                    if (ImGui::Selectable(projectionType[i], isSelected)) {
                        currentProjectionType = projectionType[i];
                        cc.Camera.SetProjectionType((SceneCamera::ProjectionType)i);
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
        });

//------------------------------SpriteComponent--------------------------------
        DrawComponent<SpriteComponent>("Sprite Renderer", entity, [](auto& sc) {
            ImGui::ColorEdit4("Color", glm::value_ptr(sc.Color));
        });
        

    }

    template<typename T,typename UIFunction>
    static void SceneHierarchyPanel::DrawComponent(const std::string& name, Entity& entity, UIFunction uiFunc) {
        const ImGuiTreeNodeFlags treeNodeFlags =
            ImGuiTreeNodeFlags_DefaultOpen |
            ImGuiTreeNodeFlags_SpanAvailWidth |     // 将点击响应区域向右延伸，即使点击文字右侧的空白也可响应
            ImGuiTreeNodeFlags_Framed |             // 让树节点（TreeNode）拥有一个完整的、带背景色的矩形框架
            ImGuiTreeNodeFlags_FramePadding |       // 让一个没有边框（Framed）的普通树节点，也应用控件框架的内边距（FramePadding），使其文本的垂直高度与标准控件（如按钮、输入框）对齐
            ImGuiTreeNodeFlags_AllowOverlap;        // 允许此节点与其他有点击事情的区域相互重叠(比如按钮)

        if (entity.HasComponent<T>())
        {
            // 设置样式
            // 获取可用区域尺寸
            ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();
            // 临时修改内边距
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4,4 });
            // 计算行高
            float lineHeight = ImGui::CalcTextSize("+").y + GImGui->Style.FramePadding.y * 2.0f;
            ImGui::Separator();// 分割线


            // 获取组件并创建该类型T组件的结点
            auto& component = entity.GetComponent<T>();
            bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());


            ImGui::PopStyleVar();// 弹出样式

            // 管理组件的按钮
            ImGui::SameLine(contentRegionAvail.x - lineHeight * 0.5f);
            if (ImGui::Button("+", { lineHeight,lineHeight })) {
                ImGui::OpenPopup("ComponentSettings");
            }
            bool removeComponent = false;
            if (ImGui::BeginPopup("ComponentSettings")) {
                if(ImGui::MenuItem("Remove component"))
                    removeComponent = true;
                ImGui::EndPopup();
            }

            // 如果创建了T组件结点
            if (open) {
                // 执行实际画ui的逻辑，一般由lamda表达式灵活提供
                uiFunc(component);
                ImGui::TreePop();   // 对应所画的name.c_str()结点
            }

            // 如果移除标志激活，就移除组件
            if (removeComponent) {
                entity.RemoveComponent<T>();
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
