#pragma once

#include"glm/glm.hpp"

#include"SoLin/Scene/SceneCamera.h"
#include"SoLin/Scene/ScriptableEntity.h"

namespace SoLin {

    //@brief 标志组件
    struct TagComponent {
        std::string Tag;    // 组件名称
        TagComponent() = default;
        TagComponent(const std::string& tag)
            :Tag(tag) {
        };
        TagComponent(const TagComponent&) = default;
    };

    //@brief 变化组件
    struct TransformComponent {
        glm::mat4 Transform{ 1.0f };// 四元数

        TransformComponent() = default;
        TransformComponent(const glm::mat4& transform)
            :Transform(transform) {
        }
        TransformComponent(const TransformComponent&) = default;				//复制函数

        // 类型转换运算符重载(方便直接以对应类型去使用该类)
        operator glm::mat4& () { return Transform; }
        operator const glm::mat4& () const { return Transform; }
    };

    //@brief 精灵组件
    struct SpriteComponent {
        glm::vec4 Color{ 1.0f,1.0f,1.0f,1.0f }; // 颜色

        SpriteComponent() = default;
        SpriteComponent(const glm::vec4& color)
            :Color(color) {
        };
        SpriteComponent(const SpriteComponent&) = default;

        operator glm::vec4& () { return Color; }
        operator const glm::vec4& ()const { return Color; }
    };

    // @brief 相机组件
    struct CameraComponent {
        SceneCamera Camera;     //场景相机
        bool Primary = false;   // 主相机标志
        bool FixedAspectRatio = false; //宽高比未变标志

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;
    };

    // @brief 原生脚本组件
    // @brief 本质上存储一个其他实体
    struct NativeScriptComponent {
        // @brief 脚本实体
        ScriptableEntity* Instance = nullptr;

        // @brief 实例化脚本实体的 函数指针
        std::function<ScriptableEntity* ()> InstantiateScript;
        // @brief 移除脚本实体的 函数指针
        std::function<void(NativeScriptComponent*)> DeinstantiateScript;

        // @brief 根据实体类型为 函数指针绑定 可调度对象
        template<typename T>
        void Bind() {
            InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
            DeinstantiateScript = [](NativeScriptComponent* nsc) {delete nsc->Instance; nsc->Instance = nullptr;}; 
        }

    };
}
