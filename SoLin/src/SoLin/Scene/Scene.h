#pragma once
#include"entt.hpp"
#include"SoLin/Core/Timestep.h"
#include"SoLin/Core/UUID.h"
#include"SoLin/Renderer/EditorCamera.h"

class b2World;

namespace SoLin {

    class Entity;

    class Scene {
    public:
        Scene();
        ~Scene();

        // @brief 复制场景
        static Ref<Scene> Copy(Ref<Scene> other);

        void OnUpdate(Timestep ts);
        // @brief 更新编辑器
        void OnUpdateEditor(Timestep ts, EditorCamera camera);
        // @brief 更新脚本
        void OnScript(Timestep ts);

        // @brief 运行 开始
        void OnRuntimeStart();
        // @brief 运行 停止
        void OnRuntimeStop();

        // @brief 视口变换尺寸回调
        void OnViewportResize(uint32_t width, uint32_t height);
        // @brief 获取主相机
        Entity GetPrimaryCamera();

        // @brief 创建实体
        Entity CreateEntity(const std::string& name = "");
        // @brief 利用UUID创建实体
        Entity CreateEntityWithUUID(UUID id, const std::string& name = "");
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

        b2World* m_PhysicsWorld = nullptr;  // Box2D世界指针

        uint32_t m_ViewportWidth, m_ViewportHeight;

        friend class Entity;
        friend class SceneHierarchyPanel;
        friend class SceneSerializer;
    };
}
