#include"slpch.h"
#include"Scene.h"

#include"SoLin/Renderer/Renderer2D.h"

#include"SoLin/Scene/Component.h"
#include"SoLin/Scene/Entity.h"

#include"glm/glm.hpp"

// Box2D
#include"box2d/b2_world.h"
#include"box2d/b2_body.h"
#include"box2d/b2_polygon_shape.h"
#include"box2d/b2_fixture.h"

namespace SoLin {

    // @brief 将组件的BodyType转换成Box2D对应类型
    static b2BodyType Rigidbody2DTypeToBox2DBody(Rigidbody2DComponent::BodyType bodyType)
    {
        switch (bodyType)
        {
        case Rigidbody2DComponent::BodyType::Static:
            return b2_staticBody;
        case Rigidbody2DComponent::BodyType::Dynamic:
            return b2_dynamicBody;
        case Rigidbody2DComponent::BodyType::Kinematic:
            return b2_kinematicBody;
        }
        SL_CORE_ASSERT(false, "Unknown body type");
        return b2_staticBody;   // 默认返回 静止体
    }

    Scene::Scene() {
    
    }

    Scene::~Scene() {

    }

    // @brief 若存在 复制原实体的对应组件
    template<typename Component>
    static void CopyComponentIfExists(Entity dstEntity, Entity srcEntity)
    {
        if (srcEntity.HasComponent<Component>())
        {
            dstEntity.AddOrReplaceComponent<Component>(srcEntity.GetComponent<Component>());
        }
    }

    // @brief 为新场景复制就场景组件
    template<typename Component>
    static void CopyComponentForNewScene(
        entt::registry& dst, entt::registry& src,
        const std::unordered_map<UUID, entt::entity>& enttMap)
    {
        // 获取原场景中所有拥有该组件的 实体
        auto view = src.view<Component>();
        for(auto e:view)
        {
            // 获取原场景中 实体的ID
            UUID uuid = src.get<IDComponent>(e).ID;
            SL_CORE_ASSERT((enttMap.find(uuid) != enttMap.end()), "Unable to find an entity in dst with an ID that matches the src entity ID(Occurs in Scene copy)");
            // 利用ID找到新场景中相应的 实体
            entt::entity dstEntity = enttMap.at(uuid);

            // 从原场景实体获取 所需组件
            auto& component = src.get<Component>(e);
            // 将组件复制到目标实体（存在则替换，不存在则添加）
            dst.emplace_or_replace<Component>(dstEntity, component);
        }
    }

    Ref<Scene> Scene::Copy(Ref<Scene> other)
    {
        // 创建新场景 获取新旧场景的注册表
        Ref<Scene> newScene = CreateRef<Scene>();
        auto& dstRegistry = newScene->m_Registry;
        auto& srcRegistry = other->m_Registry;

        newScene->m_ViewportHeight = other->m_ViewportHeight;
        newScene->m_ViewportWidth = other->m_ViewportWidth;

        std::unordered_map<UUID, entt::entity> dstEntityMap;

        // 获取原场景中所有实体
        auto& view = srcRegistry.view<IDComponent>();
        for (auto e : view) {
            // 利用原实体 创建 新实体
            UUID uuid = srcRegistry.get<IDComponent>(e).ID;
            const auto& name = srcRegistry.get<TagComponent>(e).Tag;
            Entity newEntity = newScene->CreateEntityWithUUID(uuid, name);

            // 通过UUID 建立映射
            dstEntityMap[uuid] = (entt::entity)newEntity;
        }

        // 利用映射添加组件
        CopyComponentForNewScene<TransformComponent>(dstRegistry, srcRegistry, dstEntityMap);
        CopyComponentForNewScene<CameraComponent>(dstRegistry, srcRegistry, dstEntityMap);
        CopyComponentForNewScene<SpriteComponent>(dstRegistry, srcRegistry, dstEntityMap);
        CopyComponentForNewScene<CircleComponent>(dstRegistry, srcRegistry, dstEntityMap);
        CopyComponentForNewScene<AnimationComponent>(dstRegistry, srcRegistry, dstEntityMap);
        CopyComponentForNewScene<NativeScriptComponent>(dstRegistry, srcRegistry, dstEntityMap);
        CopyComponentForNewScene<Rigidbody2DComponent>(dstRegistry, srcRegistry, dstEntityMap);
        CopyComponentForNewScene<BoxCollider2DComponent>(dstRegistry, srcRegistry, dstEntityMap);
        CopyComponentForNewScene<VelocityComponent>(dstRegistry, srcRegistry, dstEntityMap);
        CopyComponentForNewScene<PlayerComponent>(dstRegistry, srcRegistry, dstEntityMap);
        CopyComponentForNewScene<CameraControllerComponent>(dstRegistry, srcRegistry, dstEntityMap);
        CopyComponentForNewScene<LayerComponent>(dstRegistry, srcRegistry, dstEntityMap);

        return newScene;
    }

    void Scene::OnUpdate(Timestep ts)
    {
        // Box2D准备
        {
            const int velocityIterations = 6;   // 速度求解迭代速度
            const int positionIterations = 2;   // 位置求解迭代速度
            // 设置步长
            m_PhysicsWorld->Step(ts, velocityIterations, positionIterations);

            auto view = m_Registry.view<Rigidbody2DComponent>();
            for (auto e : view) {
                // 获取对应实体 来获取需要的组件
                Entity entity = { e,this };
                auto& tc = entity.GetComponent<TransformComponent>();
                auto& rb2c = entity.GetComponent<Rigidbody2DComponent>();

                // 获取Box2D刚体 来获取计算后的数据
                b2Body* body = (b2Body*)rb2c.RuntimeBody;
                b2Vec2 position = body->GetPosition();
                float angle = body->GetAngle();

                // 有速度组件就记录
                if (entity.HasComponent<VelocityComponent>()) {
                    auto& vc = entity.GetComponent<VelocityComponent>();

                    vc.Velocity.x = body->GetLinearVelocity().x;
                    vc.Velocity.y = body->GetLinearVelocity().y;
                }

                // 将需要修改的数据应用
                tc.Translation.x = position.x;
                tc.Translation.y = position.y;
                tc.Rotation.z = angle;
            }
        }


        // 渲染2D对象

        Camera* mainCamera = nullptr;
        glm::mat4 mainTransform;

        // 寻找主相机，并获取其必要组件地址
        auto view = m_Registry.view<TransformComponent,CameraComponent>();
        for (auto entity : view) {
            auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
            // 若检测到某摄像机被标记为主摄像机，则传出主摄像机的数据，然后跳出。
            if (camera.Primary)
            {
                mainCamera = &camera.Camera;
                mainTransform = transform.GetTransform();
                break;
            }
        }

        // 如果主相机存在，进行场景渲染
        if (mainCamera) {
            // 将存有投影矩阵的相机 和 其变换矩阵的逆，也就是视口矩阵，传入布景器
            Renderer2D::BeginScene(*mainCamera, glm::inverse(mainTransform));

            //// 在所有含有Transform组件的实体中找含有Sprite组件的实体，返回一个表
            //auto view = m_Registry.view<TransformComponent, SpriteComponent>();

            //for (auto entity : view) {
            //    auto [transform, sprite] = view.get<TransformComponent, SpriteComponent>(entity);

            //    Renderer2D::DrawSprite(transform.GetTransform(), sprite,(int)entity);
            //}

            //粗略写的按层渲染 后修改细节
            //TODO:
            for (int i = 0;i < 4;i++) {
                auto& group = m_Registry.group<TransformComponent, LayerComponent>(entt::get<SpriteComponent>);
                for (auto entity : group) {
                    auto& layer = group.get<LayerComponent>(entity);
                    if (layer.layer == (LayerComponent::Layer)i) {
                        auto [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);
                        //// 为层级设置z轴
                        if (layer.On)
                            transform.Translation.z = 0.01 * i;
                        Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
                    }
                }
            }

            // Circle
            {
                auto& view = m_Registry.view<TransformComponent, CircleComponent>();
                for (auto entity : view) {
                    auto [transform, circle] = view.get<TransformComponent, CircleComponent>(entity);
                    Renderer2D::DrawCircleSprite(transform.GetTransform(), circle, (int)entity);
                }
            }

            // 动画
            auto AniView = m_Registry.view<TransformComponent, AnimationComponent>();
            for (auto entity : AniView) {
                auto [transform, animation] = AniView.get<TransformComponent, AnimationComponent>(entity);
                if (animation.CurrentAnimation) {
                    Renderer2D::PlayAnimation(transform.GetTransform(), animation, (int)entity, ts);
                }
            }

            Renderer2D::EndScene();
        }
    }

    void Scene::OnUpdateEditor(Timestep ts, EditorCamera camera)
    {
        Renderer2D::BeginScene(camera);

        // 粗略写的按层渲染 后修改细节
        // TODO:
        for (int i = 0;i < 4 ;i++) {
            auto& group = m_Registry.group<TransformComponent, LayerComponent>(entt::get<SpriteComponent>);
            for (auto entity : group) {
                auto& layer = group.get<LayerComponent>(entity);
                if (layer.layer == (LayerComponent::Layer)i) {
                    auto [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);
                    //// 为层级设置z轴
                    if(layer.On)
                        transform.Translation.z = 0.01 * i;
                    Renderer2D::DrawSprite(transform.GetTransform(), sprite,(int)entity);
                }
            }
        }
        //auto& group = m_Registry.group<TransformComponent>(entt::get<SpriteComponent>);	// 在所有含有 TransformComponent 的实体中搜集含有 sprite 的实体，group 返回一个类似注册表的实体集合
        //for (auto entity : group) {
        //    auto [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);

        //    Renderer2D::DrawSprite(transform.GetTransform(), sprite,(int)entity);
        //}

        // Circle
        {
            auto& view = m_Registry.view<TransformComponent, CircleComponent>();
            for (auto entity : view) {
                auto [transform, circle] = view.get<TransformComponent, CircleComponent>(entity);
                Renderer2D::DrawCircleSprite(transform.GetTransform(), circle, (int)entity);
            }
        }

        auto AniView = m_Registry.view<TransformComponent, AnimationComponent>();
        for (auto entity : AniView) {
            auto [transform, animation] = AniView.get<TransformComponent, AnimationComponent>(entity);
            if (animation.CurrentAnimation) {
                Renderer2D::PlayAnimation(transform.GetTransform(), animation, (int)entity, ts);
            }
        }

        Renderer2D::EndScene();
    }

    void Scene::OnScript(Timestep ts)
    {
        // --------------------------------------------------------------------------------------------------------------------------------------------------------
        // 所有“含有脚本组件的实体”都将会更新其 transform，因为 ScriptableEntity::GetComponent<>() 返回 m_ScriptableEntity.GetComponent<T>(); 
        // 而这个 m_ScriptableEntity 又是 Entity 类型的，所以调用的是 Entity 的GetComponent<T>()，这就需要对 m_ScriptableEntity 进行初始化。
        // 为了使函数正常运行，在 Scene::OnScript 中，m_ScriptableEntity 被初始化为 Entity{ entity, this }。
        // 由于此时在回调函数 each 中，Entity{ entity, this }的第一个参数是有回调函数自动获取的，所以这个参数填入的 erntity 应该是正在处理的实体，也就是含有脚本组件的实体。

        // 所以说这将更新所有包含脚本组件的实体，而且每一个实体的 transform 改变的数值相同，这导致每一个实体都会受键盘影响而移动，尽管此时只有一个实体被显示出来。
        // --------------------------------------------------------------------------------------------------------------------------------------------------------

        // 更新相机控制器 脚本
        m_Registry.view<NativeScriptComponent,CameraComponent>().each
        (
            [=](auto entity, auto& nsc,auto& cc) {
                // nsc => NativeScriptController, cc => CameraComponent

                bool cameraPrimary;
                // 获取主相机标志
                auto cameraView = m_Registry.view<CameraComponent>();
                for (auto entity : cameraView) {
                    cameraPrimary = cameraView.get<CameraComponent>(entity).Primary;
                }

                // 还未实例化的话进行实例化
                if (!nsc.Instance) {
                    nsc.Instance = nsc.InstantiateScript();
                    
                    // Entity类型本质上是我们对entt::entity的封装，
                    // 其依赖Scene中的注册表，this便是将Scene指针传入
                    nsc.Instance->m_ScriptableEntity = Entity{ entity,this };

                    nsc.Instance->OnCreate();
                }
                if (cc.Primary)
                    nsc.Instance->OnUpdate(ts);
            });

        // 更新玩家控制器 脚本
        m_Registry.view<NativeScriptComponent, PlayerComponent>().each
        (
            [=](auto entity, auto& nsc, auto& pc) {
                // nsc => NativeScriptController, pc => PlayerComponent

                // 还未实例化的话进行实例化
                if (!nsc.Instance) {
                    nsc.Instance = nsc.InstantiateScript();

                    // Entity类型本质上是我们对entt::entity的封装，
                    // 其依赖Scene中的注册表，this便是将Scene指针传入
                    nsc.Instance->m_ScriptableEntity = Entity{ entity,this };

                    nsc.Instance->OnCreate();
                }
                nsc.Instance->OnUpdate(ts);
            });

    }

    void Scene::OnRuntimeStart()
    {
        // 创建物理模拟
        m_PhysicsWorld = new b2World({ 0.0f, -9.8f });

        // 为拥有刚体组件的实体创建 b2Body
        auto view = m_Registry.view<Rigidbody2DComponent>();
        for (auto e : view)
        {
            Entity entity = { e, this };								
            auto& tc = entity.GetComponent<TransformComponent>();
            auto& rb2c = entity.GetComponent<Rigidbody2DComponent>();

            b2BodyDef bodyDef;
            bodyDef.type = Rigidbody2DTypeToBox2DBody(rb2c.Type);
            bodyDef.position.Set(tc.Translation.x, tc.Translation.y);
            bodyDef.angle = tc.Rotation.z;
            b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);

            body->SetFixedRotation(rb2c.FixedRotation);
            rb2c.RuntimeBody = body;

            // 添加碰撞体
            if (entity.HasComponent<BoxCollider2DComponent>())
            {
                auto& bc2c = entity.GetComponent<BoxCollider2DComponent>();

                b2PolygonShape boxShape;
                boxShape.SetAsBox(bc2c.Size.x * tc.Scale.x, bc2c.Size.y * tc.Scale.y);

                b2FixtureDef fixtureDef;
                fixtureDef.shape = &boxShape;
                fixtureDef.density = bc2c.Density;
                fixtureDef.friction = bc2c.Friction;
                fixtureDef.restitution = bc2c.Restitution;
                fixtureDef.restitutionThreshold = bc2c.RestitutionThreshold;

                body->CreateFixture(&fixtureDef);
            }
        }
    }

    void Scene::OnRuntimeStop()
    {
        delete m_PhysicsWorld;
        m_PhysicsWorld = nullptr;
    }

    void Scene::OnViewportResize(uint32_t width, uint32_t height)
    {
        m_ViewportWidth = width;
        m_ViewportHeight = height;

        auto& view = m_Registry.view<CameraComponent>();
        for (auto entity : view) {
            auto& cameraComponent = m_Registry.get<CameraComponent>(entity);
            if (!cameraComponent.FixedAspectRatio)
                cameraComponent.Camera.ViewportResize(m_ViewportWidth, m_ViewportHeight);
        }
    }

    Entity Scene::GetPrimaryCamera()
    {
        auto view = m_Registry.view<CameraComponent>();
        for (auto entity : view) {
            const auto& cc = m_Registry.get<CameraComponent>(entity);
            if (cc.Primary == true)
                return Entity{ entity,this };
        }
        return {};
    }

    Entity Scene::CreateEntity(const std::string& name)
    {
        return CreateEntityWithUUID(UUID(), name);
    }

    Entity Scene::CreateEntityWithUUID(UUID id, const std::string& name)
    {
        Entity entity = { m_Registry.create(),this };
        entity.AddComponent<IDComponent>(id);
        entity.AddComponent<TransformComponent>(glm::vec3{ 0.0f });
        auto& tag = entity.AddComponent<TagComponent>();
        tag.Tag = name.empty() ? "Unnamed Entity" : name;
        return entity;
    }

    void Scene::DestoryEntity(Entity& entity)
    {
        // 重载过Entity的类型转换，因此可以将其直接当作entt::entity使用
        m_Registry.destroy(entity);
    }

    void Scene::DuplicateEntity(Entity& srcEntity)
    {
        std::string name = srcEntity.GetComponent<TagComponent>().Tag;
        Entity newEntity = CreateEntity(name);

        // 复制各个组件
        CopyComponentIfExists<TransformComponent>(newEntity, srcEntity);
        CopyComponentIfExists<CameraComponent>(newEntity, srcEntity);
        CopyComponentIfExists<SpriteComponent>(newEntity, srcEntity);
        CopyComponentIfExists<CircleComponent>(newEntity, srcEntity);
        CopyComponentIfExists<AnimationComponent>(newEntity, srcEntity);
        CopyComponentIfExists<NativeScriptComponent>(newEntity, srcEntity);
        CopyComponentIfExists<Rigidbody2DComponent>(newEntity, srcEntity);
        CopyComponentIfExists<BoxCollider2DComponent>(newEntity, srcEntity);
        CopyComponentIfExists<VelocityComponent>(newEntity, srcEntity);
        CopyComponentIfExists<PlayerComponent>(newEntity, srcEntity);
        CopyComponentIfExists<CameraControllerComponent>(newEntity, srcEntity);
        CopyComponentIfExists<LayerComponent>(newEntity, srcEntity);
    }

    template<typename T>
    void Scene::OnComponentAdded(Entity entity, T& component) {
        // 没被特化就是不存在，进行静态断言
        static_assert(false);
    }
    template<>
    void Scene::OnComponentAdded<IDComponent>
        (Entity entity,IDComponent& component){}
    template<>
    void Scene::OnComponentAdded<TagComponent>
        (Entity entity,TagComponent& component){}
    template<>
    void Scene::OnComponentAdded<TransformComponent>
        (Entity entity, TransformComponent& component){}
    template<>
    void Scene::OnComponentAdded<SpriteComponent>
        (Entity entity, SpriteComponent& component){}
    template<>
    void Scene::OnComponentAdded<CircleComponent>
        (Entity entity, CircleComponent& component) {}
    template<>
    void Scene::OnComponentAdded<AnimationComponent>
        (Entity entity,AnimationComponent& component)
    {
        // 有精灵就移除
        if (entity.HasComponent<SpriteComponent>())
            entity.RemoveComponent<SpriteComponent>();
    }
    template<>
    void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
    {
        // 相机组件需要更新其视口大小
        component.Camera.ViewportResize(m_ViewportWidth, m_ViewportHeight);
    }
    template<>
    void Scene::OnComponentAdded<NativeScriptComponent>
        (Entity entity, NativeScriptComponent& component){}
    template<>
    void Scene::OnComponentAdded<Rigidbody2DComponent>
        (Entity entity, Rigidbody2DComponent& component) {
    }
    template<>
    void Scene::OnComponentAdded<BoxCollider2DComponent>
        (Entity entity, BoxCollider2DComponent& component) {
    }
    template<>
    void Scene::OnComponentAdded<VelocityComponent>
        (Entity entity, VelocityComponent& component) {
    }
    template<>
    void Scene::OnComponentAdded<PlayerComponent>
        (Entity entity, PlayerComponent& component) {
        // 为其添加玩家控制器
        entity.AddComponent<NativeScriptComponent>().Bind<ScriptPlayerController>();
    }
    template<>
    void Scene::OnComponentAdded<CameraControllerComponent>
        (Entity entity, CameraControllerComponent& component) {
        // 为其添加相机控制器
        entity.AddComponent<NativeScriptComponent>().Bind<ScriptCameraController>();
    }
    template<>
    void Scene::OnComponentAdded<LayerComponent>
        (Entity entity, LayerComponent& component) {
    }
    template<>
    void Scene::OnComponentAdded<MoveComponent>
        (Entity entity, MoveComponent& component) {
    }
}
