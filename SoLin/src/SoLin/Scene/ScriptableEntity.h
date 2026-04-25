#pragma once

#include"SoLin/Scene/Entity.h"

namespace SoLin {
    // @brief 脚本实体
    // @brief 用于运行时添加其他实体
    class ScriptableEntity {
    public:
        virtual ~ScriptableEntity() = default;

        // @brief 获取模板类型组件
        template<typename T>
        T& GetComponent() {
            return m_ScriptableEntity.GetComponent<T>();
        }
        // @brief 判断持有 模板类型 组件
        template<typename T>
        bool HasComponent() {
            return m_ScriptableEntity.HasComponent<T>();
        }
        // @brief 添加 模板类型 组件
        template<typename T, typename... Args>
        T& AddComponent(Args&&... args) {
            return m_ScriptableEntity.AddOrReplaceComponent<T>(std::forward<Args>(args)...);
        }
        // @brief 移除 模板类型 组件
        template<typename T>
        void RemoveComponent() {
            m_ScriptableEntity.RemoveComponent<T>();
        }

    private:
        friend class Scene;
        Entity m_ScriptableEntity;
    protected:
        virtual void OnCreate() = 0;
        virtual void OnDestroy() = 0;
        virtual void OnUpdate(Timestep ts) = 0;
    };

    //@brief 相机控制器脚本
    class ScriptCameraController :public ScriptableEntity {
    public:
        virtual void OnCreate() override;
        virtual void OnDestroy()override;
        virtual void OnUpdate(Timestep ts)override;
    };

    // @brief 玩家控制器脚本
    class ScriptPlayerController :public ScriptableEntity {
    public:
        virtual void OnCreate() override;
        virtual void OnDestroy()override;
        virtual void OnUpdate(Timestep ts)override;
    };
}
