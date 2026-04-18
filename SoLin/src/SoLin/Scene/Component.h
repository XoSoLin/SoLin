#pragma once

#include"glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include"glm/gtx/quaternion.hpp"

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
        // @brief 位置
        glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
        // @brief 旋转
        glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
        // @brief 缩放
        glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

        TransformComponent() = default;
        TransformComponent(const glm::vec3& translation)
            :Translation(translation) {
        }
        TransformComponent(const TransformComponent&) = default;				//复制函数

        // 类型转换运算符重载(方便直接以对应类型去使用该类)
        operator glm::mat4& () { return GetTransform(); }
        operator const glm::mat4& () const { return GetTransform(); }

        glm::mat4 GetTransform() const
        {
            /*glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, { 1.0f, 0.0f, 0.0f })
                * glm::rotate(glm::mat4(1.0f), Rotation.y, { 0.0f, 1.0f, 0.0f })
                * glm::rotate(glm::mat4(1.0f), Rotation.z, { 0.0f, 0.0f, 1.0f });*/
            glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

            return glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scale);
        }
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
