#pragma once

#include<entt.hpp>

#include"SoLin/Scene/Scene.h"

namespace SoLin {

    class Entity {
    public:
        Entity() = default;
        Entity(entt::entity entityHandle, Scene* scene);
        Entity(const Entity& other) = default;

        ~Entity() = default;

        // @brief 添加组件 模板
        template<typename T,typename... Args>
        T& AddComponent(Args&&... args) {
            SL_CORE_ASSERT(!HasComponent<T>(), "This Entity already has component!");
            return m_Scene->Reg().emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
        }

        // @brief 获取组件 模板
        template<typename T>
        T& GetComponent()
        {
            SL_CORE_ASSERT(HasComponent<T>(), "This Entity does not have component!");
            return m_Scene->Reg().get<T>(m_EntityHandle);
        }

        // @brief 移除组件 模板
        template<typename T>
        void RemoveComponent() {
            SL_CORE_ASSERT(HasComponent<T>(), "This Entity does not have component!");
            m_Scene->Reg().remove<T>(m_EntityHandle);
        }

        // @brief 持有组件判断 模板
        template<typename T>
        bool HasComponent() {
            return m_Scene->Reg().all_of<T>(m_EntityHandle);
        }

        // @brief 重载bool类型转换
        operator bool() const { return m_EntityHandle != entt::null; }
    private:
        entt::entity m_EntityHandle{ entt::null };  // 实体
        Scene* m_Scene = nullptr;                   // 所在场景指针
    };
}
