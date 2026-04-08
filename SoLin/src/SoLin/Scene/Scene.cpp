#include"slpch.h"
#include"Scene.h"

#include"SoLin/Renderer/Renderer2D.h"

#include"SoLin/Scene/Component.h"
#include"SoLin/Scene/Entity.h"

#include"glm/glm.hpp"

namespace SoLin {

    Scene::Scene() {
    
    }

    Scene::~Scene() {

    }

    void Scene::OnUpdate(Timestep ts)
    {
        Camera* mainCamera = nullptr;
        glm::mat4* mainTransform = nullptr;

        // 寻找主相机，并获取其必要组件地址
        auto view = m_Registry.view<TransformComponent,CameraComponent>();
        for (auto entity : view) {
            auto& [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
            // 若检测到某摄像机被标记为主摄像机，则传出主摄像机的数据，然后跳出。
            if (camera.Primary)
            {
                mainCamera = &camera.Camera;
                mainTransform = &transform.Transform;
                break;
            }
        }

        // 如果主相机存在，进行场景渲染
        if (mainCamera) {
            // 将存有投影矩阵的相机 和 其变换矩阵的逆，也就是视口矩阵，传入布景器
            Renderer2D::BeginScene(*mainCamera, glm::inverse(*mainTransform));

            // 在所有含有Transform组件的实体中找含有Sprite组件的实体，返回一个表
            auto view = m_Registry.view<TransformComponent, SpriteComponent>();

            for (auto entity : view) {
                auto& [transform, color] = view.get<TransformComponent, SpriteComponent>(entity);

                Renderer2D::DrawQuad(transform, color);
            }

            Renderer2D::EndScene();
        }
    }

    Entity Scene::CreateEntity(const std::string& name)
    {
        Entity entity = { m_Registry.create(),this };
        entity.AddComponent<TransformComponent>(glm::mat4{ 1.0f });
        auto& tag = entity.AddComponent<TagComponent>();
        tag.Tag = name.empty() ? "Unnamed Entity" : name;
        return entity;
    }
}
