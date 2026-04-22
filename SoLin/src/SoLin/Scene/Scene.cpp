#include"slpch.h"
#include"Scene.h"

#include"SoLin/Renderer/Renderer2D.h"

#include"SoLin/Scene/Component.h"
#include"SoLin/Scene/Entity.h"

#include"glm/glm.hpp"

namespace SoLin {

    Scene::Scene() {
    
    }

    Scene::~Scene() {

    }

    void Scene::OnUpdate(Timestep ts)
    {
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

            // 在所有含有Transform组件的实体中找含有Sprite组件的实体，返回一个表
            auto view = m_Registry.view<TransformComponent, SpriteComponent>();

            for (auto entity : view) {
                auto [transform, sprite] = view.get<TransformComponent, SpriteComponent>(entity);

                Renderer2D::DrawSprite(transform.GetTransform(), sprite,(int)entity);
            }

            Renderer2D::EndScene();
        }
    }

    void Scene::OnUpdateEditor(Timestep ts, EditorCamera camera)
    {
        Renderer2D::BeginScene(camera);

        auto& group = m_Registry.group<TransformComponent>(entt::get<SpriteComponent>);	// 在所有含有 TransformComponent 的实体中搜集含有 sprite 的实体，group 返回一个类似注册表的实体集合
        for (auto entity : group) {
            auto [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);

            Renderer2D::DrawSprite(transform.GetTransform(), sprite,(int)entity);
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

        // 更新脚本
        m_Registry.view<NativeScriptComponent,CameraComponent>().each
        (
            [=](auto entity, auto& nsc,auto& cc) {
                // nsc => NativeScriptController, cc => CameraController

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
        Entity entity = { m_Registry.create(),this };
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

    template<typename T>
    void Scene::OnComponentAdded(Entity entity, T& component) {
        // 没被特化就是不存在，进行静态断言
        static_assert(false);
    }
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
}
