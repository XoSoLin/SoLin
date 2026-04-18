#pragma once
#include"entt.hpp"
#include"SoLin/Core/Timestep.h"

namespace SoLin {

    class Entity;

    class Scene {
    public:
        Scene();
        ~Scene();

        void OnUpdate(Timestep ts);
        void OnScript(Timestep ts);

        // @brief 视口变换尺寸回调
        void OnViewportResize(uint32_t width, uint32_t height);
        // @brief 获取主相机
        Entity GetPrimaryCamera();

        // @brief 创建实体
        Entity CreateEntity(const std::string& name = "");
        // @brief 移除实体
        void DestoryEntity(Entity& entity);

        // @brief 返回注册表
        entt::registry& Reg() { return m_Registry; }

    private:
        // @brief 添加组件回调
        template<typename T>
        void OnComponentAdded(Entity entity, T& component);

    private:
        entt::registry m_Registry;

        uint32_t m_ViewportWidth, m_ViewportHeight;

        friend class Entity;
        friend class SceneHierarchyPanel;
        friend class SceneSerializer;
    };
}
