#pragma once

#include"SoLin/Core/UUID.h"
#include"SoLin/Renderer/Texture.h"
#include"SoLin/Scene/SceneCamera.h"
#include"SoLin/Scene/Animation/Animation.h"
#include"SoLin/Scene/ScriptableEntity.h"


#include"glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include"glm/gtx/quaternion.hpp"


namespace SoLin {

    struct IDComponent {
        UUID ID;

        IDComponent() = default;
        IDComponent(const UUID& id)
            :ID(id){ }
        IDComponent(const IDComponent&) = default;
    };

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
        Ref<Texture2D> Texture; // 纹理
        float TilingFactor = 1.0f;// 填充因子

        SpriteComponent() = default;
        SpriteComponent(const glm::vec4& color)
            :Color(color) {
        };
        SpriteComponent(const SpriteComponent&) = default;

        operator glm::vec4& () { return Color; }
        operator const glm::vec4& ()const { return Color; }
    };

    // @brief 动画组件
    struct AnimationComponent {
        // 动画映射
        std::unordered_map<std::string, Ref<Animation>> AnimationMap;
        Ref<Animation> CurrentAnimation;// 当前动画

        float AnimationTimer = 0.0f;    // 动画计时器
        bool IsPlaying = false;         // 正在播放标志
        bool IsOne = false;             // 一次性标志

        AnimationComponent() = default;
        AnimationComponent(const AnimationComponent&) = default;
        AnimationComponent(Ref<Animation> animation)
            :CurrentAnimation(animation){
            AnimationMap[animation->getName()] = animation;
        }
    };

    // @brief 相机组件
    struct CameraComponent {
        SceneCamera Camera;     //场景相机
        bool Primary = false;   // 主相机标志
        bool FixedAspectRatio = false; //宽高比未变标志

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;
    };

    //---------------------------------------Script------------------------------------

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

    //--------------------------Physics------------------------------------

    // @brief 刚体2D组件
    struct Rigidbody2DComponent {

        // @brief 类型
        enum class BodyType{
            Static = 0,     // 静止
            Dynamic,        // 动态
            Kinematic       // 运动
        };
        BodyType Type = BodyType::Static;   // 默认静止
        bool FixedRotation = false;         // 固定旋转

        void* RuntimeBody = nullptr;

        Rigidbody2DComponent() = default;
        Rigidbody2DComponent(const Rigidbody2DComponent&) = default;

    };

    // 2D碰撞盒组件
    struct BoxCollider2DComponent {
        glm::vec2 Offset = { 0.0f, 0.0f };					// 重心偏移量
        glm::vec2 Size = { 0.5f, 0.5f };					// 碰撞体积

        float Density = 1.0f;								// 密度
        float Friction = 0.5f;								// 摩擦力
        float Restitution = 0.0f;							// 恢复
        float RestitutionThreshold = 0.5f;					// 恢复阈值

        void* RuntimeFixture = nullptr;						// 运行时的配件、运行时附件

        BoxCollider2DComponent() = default;
        BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
    };


    //----------------------------game-------------------------------------------
    struct VelocityComponent {
        glm::vec3 Velocity = { 0.0f,0.0f,0.0f };
        // 目前 x代表水平最大速度，y代表水平起跳瞬时速度
        glm::vec3 MaxVelocity = { 4.0f,2.0f,0.0f };

        VelocityComponent() = default;
        VelocityComponent(const glm::vec3& velocity)
            :MaxVelocity(velocity) {
        }
        VelocityComponent(const glm::vec2& velocity)
            :MaxVelocity({velocity.x,velocity.y,0.0f}) {
        }
        VelocityComponent(const VelocityComponent&) = default;

        operator glm::vec3& () { return Velocity; }
        operator const glm::vec3& ()const { return Velocity; }
        operator glm::vec2& () { return glm::vec2(Velocity.x ,Velocity.y); }
        operator const glm::vec2& ()const { return { Velocity.x ,Velocity.y }; }
    };

    // @brief 玩家组件
    struct PlayerComponent {
        enum class Mode {
            Move,
            Jump,
            Attack,
            Fill,
            Hind
        };
        Mode mode = Mode::Move;
        PlayerComponent() = default;
        PlayerComponent(const PlayerComponent&) = default;
    };

    // @brief 相机控制组件
    struct CameraControllerComponent {
        enum class Mode {
            Move
        };
        Mode mode = Mode::Move;
        CameraControllerComponent() = default;
        CameraControllerComponent(const CameraControllerComponent&) = default;
    };


    // 标志组件 不需要参与 序列化与复制
    struct MoveComponent {
        enum class Mode {
            Left,
            Right,
            Up,
            Down
        };
        Mode mode = Mode::Down;
    };

}
