#include"slpch.h"
#include"Scene.h"

#include"Component.h"
#include"SoLin/Renderer/Renderer2D.h"

#include"glm/glm.hpp"

namespace SoLin {

    Scene::Scene() {
    
    }

    Scene::~Scene() {

    }

    void Scene::OnUpdate(Timestep ts)
    {
        // 在所有含有Transform组件的实体中找含有Sprite组件的实体，返回一个表
        auto group = m_Registry.group<TransformComponent>(entt::get<SpriteComponent>);

        for (auto entity : group) {
            auto& [transform, color] = group.get<TransformComponent, SpriteComponent>(entity);

            Renderer2D::DrawQuad(transform, color);
        }
    }

    entt::entity Scene::CreateEntity()
    {
        return m_Registry.create();
    }
}
