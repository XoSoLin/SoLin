#include "slpch.h"
#include "SoLin/Scene/ScriptableEntity.h"

#include "SoLin/Scene/Component.h"

#include "SoLin/Core/KeyCodes.h"
#include "SoLin/Core/Input.h"

// Box2D
#include"box2d/b2_body.h"

namespace SoLin {
    void ScriptableEntity::OnCreate()
    {
    }

    void ScriptableEntity::OnDestroy()
    {
    }

    void ScriptableEntity::OnUpdate(Timestep ts)
    {
    }

    // ---------------------------------------------------------------------------------------------------------------------

    void ScriptCameraController::OnCreate()
    {
    }

    void ScriptCameraController::OnDestroy()
    {
    }

    void ScriptCameraController::OnUpdate(Timestep ts)
    {
        // --------------------------------------------------------------------------------------------------------------------------------------------------------
        // 所有“含有脚本组件的实体”都将会更新其 transform，因为 ScriptableEntity::GetComponent<>() 返回 m_ScriptableEntity.GetComponent<T>(); 
        // 而这个 m_ScriptableEntity 又是 Entity 类型的，所以调用的是 Entity 的GetComponent<T>()，这就需要对 m_ScriptableEntity 进行初始化。
        // 为了使函数正常运行，在 Scene::OnScript 中，m_ScriptableEntity 被初始化为 Entity{ entity, this }。
        // 由于此时在回调函数 each 中，Entity{ entity, this }的第一个参数是有回调函数自动获取的，所以这个参数填入的 erntity 应该是正在处理的实体，也就是含有脚本组件的实体。

        // 所以说这将更新所有包含脚本组件的实体，而且每一个实体的 transform 改变的数值相同，这导致每一个实体都会受键盘影响而移动，尽管此时只有一个实体被显示出来。
        // --------------------------------------------------------------------------------------------------------------------------------------------------------
        auto& translation = GetComponent<TransformComponent>().Translation;
        float speed = 5.0f;

        if (Input::IsKeyPressed(SL_KEY_A))
            translation.x -= speed * ts;
        if (Input::IsKeyPressed(SL_KEY_D))
            translation.x += speed * ts;
        if (Input::IsKeyPressed(SL_KEY_W))
            translation.y += speed * ts;
        if (Input::IsKeyPressed(SL_KEY_S))
            translation.y -= speed * ts;


    }


//----------------------------Player------------------------------------------------------
    void ScriptPlayerController::OnCreate()
    {
        auto a = CreateRef<Animation>();
        a->SetName("Stop");
        a->AddFrame(SubTexture2D::Create(Texture2D::Create("assets/textures/su.png"), { 66,66 }, { 0,11 }), 0.1f);
        a->AddFrame(SubTexture2D::Create(Texture2D::Create("assets/textures/su.png"), { 66,66 }, { 1,11 }), 0.1f);
        a->AddFrame(SubTexture2D::Create(Texture2D::Create("assets/textures/su.png"), { 66,66 }, { 2,11 }), 0.1f);
        a->AddFrame(SubTexture2D::Create(Texture2D::Create("assets/textures/su.png"), { 66,66 }, { 3,11 }), 0.1f);
        a->AddFrame(SubTexture2D::Create(Texture2D::Create("assets/textures/su.png"), { 66,66 }, { 4,11 }), 0.1f);
        a->AddFrame(SubTexture2D::Create(Texture2D::Create("assets/textures/su.png"), { 66,66 }, { 5,11 }), 0.1f);
        a->AddFrame(SubTexture2D::Create(Texture2D::Create("assets/textures/su.png"), { 66,66 }, { 6,11 }), 0.1f);
        a->AddFrame(SubTexture2D::Create(Texture2D::Create("assets/textures/su.png"), { 66,66 }, { 7,11 }), 0.1f);

        auto& anC = AddComponent<AnimationComponent>(a);

        auto a1 = CreateRef<Animation>();
        a1->SetName("Move");
        a1->AddFrame(SubTexture2D::Create(Texture2D::Create("assets/textures/su.png"), { 66,66 }, { 0,10 }), 0.1f);
        a1->AddFrame(SubTexture2D::Create(Texture2D::Create("assets/textures/su.png"), { 66,66 }, { 1,10 }), 0.1f);
        a1->AddFrame(SubTexture2D::Create(Texture2D::Create("assets/textures/su.png"), { 66,66 }, { 2,10 }), 0.1f);
        a1->AddFrame(SubTexture2D::Create(Texture2D::Create("assets/textures/su.png"), { 66,66 }, { 3,10 }), 0.1f);
        a1->AddFrame(SubTexture2D::Create(Texture2D::Create("assets/textures/su.png"), { 66,66 }, { 4,10 }), 0.1f);
        a1->AddFrame(SubTexture2D::Create(Texture2D::Create("assets/textures/su.png"), { 66,66 }, { 5,10 }), 0.1f);
        a1->AddFrame(SubTexture2D::Create(Texture2D::Create("assets/textures/su.png"), { 66,66 }, { 6,10 }), 0.1f);
        a1->AddFrame(SubTexture2D::Create(Texture2D::Create("assets/textures/su.png"), { 66,66 }, { 7,10 }), 0.1f);
        a1->AddFrame(SubTexture2D::Create(Texture2D::Create("assets/textures/su.png"), { 66,66 }, { 8,10 }), 0.1f);
        a1->AddFrame(SubTexture2D::Create(Texture2D::Create("assets/textures/su.png"), { 66,66 }, { 9,10 }), 0.1f);
        a1->AddFrame(SubTexture2D::Create(Texture2D::Create("assets/textures/su.png"), { 66,66 }, { 10,10 }), 0.1f);
        a1->AddFrame(SubTexture2D::Create(Texture2D::Create("assets/textures/su.png"), { 66,66 }, { 11,10 }), 0.1f);
        anC.AnimationMap[a1->getName()] = a1;

        auto a2 = CreateRef<Animation>();
        a2->SetName("Jump");
        a2->AddFrame(SubTexture2D::Create(Texture2D::Create("assets/textures/su.png"), { 66,66 }, { 0,8 }), 0.1f);
        a2->AddFrame(SubTexture2D::Create(Texture2D::Create("assets/textures/su.png"), { 66,66 }, { 1,8 }), 0.1f);
        a2->AddFrame(SubTexture2D::Create(Texture2D::Create("assets/textures/su.png"), { 66,66 }, { 2,8 }), 0.1f);
        a2->AddFrame(SubTexture2D::Create(Texture2D::Create("assets/textures/su.png"), { 66,66 }, { 3,8 }), 0.1f);
        a2->AddFrame(SubTexture2D::Create(Texture2D::Create("assets/textures/su.png"), { 66,66 }, { 4,8 }), 0.1f);
        a2->AddFrame(SubTexture2D::Create(Texture2D::Create("assets/textures/su.png"), { 66,66 }, { 5,8 }), 0.1f);
        anC.AnimationMap[a2->getName()] = a2;

        auto a3 = CreateRef<Animation>();
        a3->SetName("Hover");
        a3->AddFrame(SubTexture2D::Create(Texture2D::Create("assets/textures/su.png"), { 66,66 }, { 5,8 }), 0.1f);
        anC.AnimationMap[a3->getName()] = a3;
    }

    void ScriptPlayerController::OnDestroy()
    {
    }

    void ScriptPlayerController::OnUpdate(Timestep ts)
    {
        auto& tc = GetComponent<TransformComponent>();
        auto& playerState = GetComponent<PlayerComponent>().mode;
        auto& vc = GetComponent<VelocityComponent>();

        auto& rb2c = GetComponent<Rigidbody2DComponent>();
        b2Body* body = (b2Body*)rb2c.RuntimeBody;

        switch (playerState) {

        case PlayerComponent::Mode::Stop:// 可切换到 移动 跳跃 滞空
            if (Input::IsKeyPressed(SL_KEY_A) || Input::IsKeyPressed(SL_KEY_D))
                playerState = PlayerComponent::Mode::Move;
            if (Input::IsKeyPressed(SL_KEY_W)) {
                // -起跳操作-
                float froceY = body->GetMass() * (vc.MaxVelocity.y / ts);
                // 将力添加到物体瞬时跳起
                body->ApplyForceToCenter({ 0.0f,froceY }, true);

                playerState = PlayerComponent::Mode::Jump;
                break;
            }
            if (vc.Velocity.y > 0.1 || vc.Velocity.y < -0.1)
                playerState = PlayerComponent::Mode::Hover;
            break;

        case PlayerComponent::Mode::Jump:// 可切换到 滞空
            // 跳跃状态下可以左右移动
            // -移动操作-
            {
                float targetSpeed = vc.MaxVelocity.x;
                float VelocityX = 0;
                if (Input::IsKeyPressed(SL_KEY_A))
                    VelocityX = -targetSpeed;
                if (Input::IsKeyPressed(SL_KEY_D))
                    VelocityX = targetSpeed;
                float currentVelocityX = body->GetLinearVelocity().x;
                float velError = VelocityX - currentVelocityX;
                float froceX = body->GetMass() * (velError / ts);
                // 将力添加到物体
                body->ApplyForceToCenter({ froceX,0.0f }, true);
            }

            // -判断滞空操作-
            if(vc.Velocity.y<0)
                playerState = PlayerComponent::Mode::Hover;
            break;

        case PlayerComponent::Mode::Move:// 可切换到 待机 跳跃 攻击
            // -移动和起跳操作-
            {
                // 获取水平方向的目标速度值
                float targetSpeed = vc.MaxVelocity.x;
                float froceY = 0;
                // 计算目标水平方向速度
                float VelocityX = 0;
                if (Input::IsKeyPressed(SL_KEY_A))
                    VelocityX = -targetSpeed;
                if (Input::IsKeyPressed(SL_KEY_D))
                    VelocityX = targetSpeed;
                if (Input::IsKeyPressed(SL_KEY_W))
                    froceY = body->GetMass() * (vc.MaxVelocity.y / ts);

                // 获取当前水平方向速度
                float currentVelocityX = body->GetLinearVelocity().x;
                // 计算当前速度误差(距离目标还差多少)
                float velError = VelocityX - currentVelocityX;
                // 根据误差计算力 f = m*a = m *(dtV/dtT)
                float froceX = body->GetMass() * (velError / ts);
                // 将力添加到物体
                body->ApplyForceToCenter({ froceX,froceY }, true);
                if (froceY != 0)
                    playerState = PlayerComponent::Mode::Jump;
                if (VelocityX == 0)
                    playerState = PlayerComponent::Mode::Stop;
            }
            //// -判断待机操作-
            //if(vc.Velocity.x==0)
            //    playerState = PlayerComponent::Mode::Stop;
            break;
        case PlayerComponent::Mode::Attack:// 可切换到 待机
            break;
        case PlayerComponent::Mode::Hover:// 可切换到 待机
            // -移动操作-
            {
                float targetSpeed = vc.MaxVelocity.x;
                float VelocityX = 0;
                if (Input::IsKeyPressed(SL_KEY_A))
                    VelocityX = -targetSpeed;
                if (Input::IsKeyPressed(SL_KEY_D))
                    VelocityX = targetSpeed;
                float currentVelocityX = body->GetLinearVelocity().x;
                float velError = VelocityX - currentVelocityX;
                float froceX = body->GetMass() * (velError / ts);
                // 将力添加到物体
                body->ApplyForceToCenter({ froceX,0.0f }, true);
            }

            // -判断着地操作-
            if (vc.Velocity.y == 0)
                playerState = PlayerComponent::Mode::Stop;
            break;
        }

        auto& ac = GetComponent<AnimationComponent>();
        switch (playerState) {
        case PlayerComponent::Mode::Stop:
            ac.CurrentAnimation = ac.AnimationMap["Stop"];
            break;
        case PlayerComponent::Mode::Jump:
            ac.CurrentAnimation = ac.AnimationMap["Jump"];
            break;
        case PlayerComponent::Mode::Move:
            ac.CurrentAnimation = ac.AnimationMap["Move"];
            break;
        case PlayerComponent::Mode::Hover:
            ac.CurrentAnimation = ac.AnimationMap["Hover"];
            break;
        case PlayerComponent::Mode::Attack:
            ac.CurrentAnimation = ac.AnimationMap["Attack"];
            break;
        }


#if 0
        // 如果有2D盒组件就进行物理移动
        if (HasComponent<Rigidbody2DComponent>()) {
            auto& rb2c = GetComponent<Rigidbody2DComponent>();

            b2Body* body = (b2Body*)rb2c.RuntimeBody;

            // 有速度组件就可以左右移动
            if (HasComponent<VelocityComponent>()) {
                auto& vc = GetComponent<VelocityComponent>();

                // 获取水平方向的目标速度值
                float targetSpeed = vc.MaxVelocity.x;
                float froceY = 0;
                // 计算目标水平方向速度
                float VelocityX = 0;
                if (Input::IsKeyPressed(SL_KEY_A))
                    VelocityX = -targetSpeed;
                if (Input::IsKeyPressed(SL_KEY_D))
                    VelocityX = targetSpeed;
                if (Input::IsKeyPressed(SL_KEY_W))
                    froceY = body->GetMass() * (vc.MaxVelocity.y / ts);
                
                // 获取当前水平方向速度
                float currentVelocityX = body->GetLinearVelocity().x;
                // 计算当前速度误差(距离目标还差多少)
                float velError = VelocityX - currentVelocityX;
                // 根据误差计算力 f = m*a = m *(dtV/dtT)
                float froceX = body->GetMass() * (velError / ts);
                // 将力添加到物体
                body->ApplyForceToCenter({ froceX,froceY }, true);
            }

        }
        // 没有的情况下就直接进行位置移动
        else {
            auto& translation = GetComponent<TransformComponent>().Translation;
            float speed = 5.0f;

            if (Input::IsKeyPressed(SL_KEY_A))
                translation.x -= speed * ts;
            if (Input::IsKeyPressed(SL_KEY_D))
                translation.x += speed * ts;
            if (Input::IsKeyPressed(SL_KEY_W))
                translation.y += speed * ts;
            if (Input::IsKeyPressed(SL_KEY_S))
                translation.y -= speed * ts;
        }
#endif

    }

}
