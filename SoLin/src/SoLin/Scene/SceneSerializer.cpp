#include "slpch.h"
#include "SoLin/Scene/SceneSerializer.h"

#include"SoLin/Scene/Entity.h"
#include"SoLin/Scene/Component.h"

namespace YAML {

    // 进行特化   为 glm::vec3 类型提供 YAML 转换支持
    template<>
    struct convert<glm::vec3> {
        // 编码
        static Node encode(const glm::vec3& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            return node;
        }

        // 解码
        static bool decode(const Node& node, glm::vec3& rhs) {
            // 查看结点是否为序列类型，且结点数为3，不符合返回F
            if (!node.IsSequence() || node.size() != 3) {
                return false;
            }

            // 完成数据解码，返回T
            rhs.x = node[0].as<double>();
            rhs.y = node[1].as<double>();
            rhs.z = node[2].as<double>();
            return true;
        }
    };

    // 进行特化   为 glm::vec4 类型提供 YAML 转换支持
    template<>
    struct convert<glm::vec4> {
        static Node encode(const glm::vec4& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);
            return node;
        }

        static bool decode(const Node& node, glm::vec4& rhs) {
            if (!node.IsSequence() || node.size() != 4) {
                return false;
            }

            rhs.x = node[0].as<double>();
            rhs.y = node[1].as<double>();
            rhs.z = node[2].as<double>();
            rhs.w = node[3].as<double>();
            return true;
        }
    };


}

namespace SoLin {

    YAML::Emitter& operator<<(YAML::Emitter& out, glm::vec3& vec)
    {
        out << YAML::Flow;                                              // Set the output format to streaming format instead of block format
        out << YAML::BeginSeq << vec.x << vec.y << vec.z << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, glm::vec4& vec)       // We use it in "SerializeEntity() -> YAML::Value << color"
    {
        out << YAML::Flow;                                              // Set the output format to streaming format instead of block format
        out << YAML::BeginSeq << vec.x << vec.y << vec.z << vec.w << YAML::EndSeq;
        return out;
    }

    SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
        :m_Scene(scene) {

    }

    void SceneSerializer::Serialize(const std::string& filepath)
    {
        // 创建 YAML 发射器，用于构建YAML格式的字符串
        YAML::Emitter out;
        out << YAML::BeginMap;          // 开始一个映射
        out << YAML::Key << "Scene" << YAML::Value << "Untitled";
        out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
        // 为实体们 开始一个序列
        m_Scene->m_Registry.view<entt::entity>().each([&](auto entityID) {

            Entity entity{ entityID,m_Scene.get() };
            if (!entity)
                return;

            SerializeEntity(out, entity);

        });
        out << YAML::EndSeq;    // 结束序列
        out << YAML::EndMap;    // 结束映射

        // 创建一个输出流，并将序列化的内容注入其中
        std::ofstream fout(filepath);
        fout << out.c_str();

    }

    void SceneSerializer::SerializeRuntime(const std::string& filepath)
    {
        SL_CORE_ASSERT(false, "SerializeRuntime is not implemented yet");
    }

    bool SceneSerializer::Deserialize(const std::string& filepath)
    {
        // 以输入流形式打开文件，并将其全部内容暂存到字符流中
        std::ifstream file(filepath);
        std::stringstream fileContent;
        fileContent << file.rdbuf();

        // 从字符流中加载YAML数据，解析为Node对象
        YAML::Node data = YAML::Load(fileContent);
        // 查看是否有Scene键
        if (!data["Scene"])
            return false;

        std::string sceneName = data["Scene"].as<std::string>();
        SL_CORE_TRACE("Deserializing scene '{0}'", sceneName);

        auto entites = data["Entities"];
        if (entites) {
            for (auto entity : entites) {
                uint64_t uuid;
                std::string name;

                uuid = entity["Entity"].as<uint64_t>();

                auto tc = entity["TagComponent"];
                if (tc)
                    name = tc["Tag"].as<std::string>();

                SL_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name);
                Entity& deserializedEntity = m_Scene->CreateEntity(name);

                DeserializeEntity(entity, deserializedEntity);
            }
        }
        return true;
    }

    bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
    {
        SL_CORE_ASSERT(false, "DeserializeRuntime is not implemented yet");
        return false;
    }

    void SceneSerializer::SerializeEntity(YAML::Emitter& out, Entity& entity)
    {
        out << YAML::BeginMap; // Entity
        out << YAML::Key << "Entity" << YAML::Value << "410";          // TODO: Entity ID goes here

        if (entity.HasComponent<TagComponent>())
        {
            out << YAML::Key << "TagComponent";
            out << YAML::BeginMap;

            auto& tag = entity.GetComponent<TagComponent>().Tag;
            out << YAML::Key << "Tag" << YAML::Value << tag;

            out << YAML::EndMap;
        }

        if (entity.HasComponent<TransformComponent>())
        {
            out << YAML::Key << "TransformComponent";
            out << YAML::BeginMap;

            auto& tc = entity.GetComponent<TransformComponent>();
            out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
            out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
            out << YAML::Key << "Scale" << YAML::Value << tc.Scale;

            out << YAML::EndMap;
        }

        if (entity.HasComponent<CameraComponent>())
        {
            out << YAML::Key << "CameraComponent";
            out << YAML::BeginMap;

            auto& cc = entity.GetComponent<CameraComponent>();
            auto& camera = cc.Camera;

            out << YAML::Key << "Camera" << YAML::Value;
            out << YAML::BeginMap; {
                out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
                out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveVerticalFOV();
                out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
                out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();
                out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
                out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip();
                out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip();
                out << YAML::EndMap;
            }
            out << YAML::Key << "Primary" << YAML::Value << cc.Primary;
            out << YAML::Key << "Fixed Aspect Ratio" << YAML::Value << cc.FixedAspectRatio;

            out << YAML::EndMap;
        }

        if (entity.HasComponent<SpriteComponent>())
        {
            out << YAML::Key << "SpriteComponent";
            out << YAML::BeginMap;

            auto& color = entity.GetComponent<SpriteComponent>().Color;
            out << YAML::Key << "Color" << YAML::Value << color;

            out << YAML::EndMap;
        }

        out << YAML::EndMap;
    }

    void SceneSerializer::DeserializeEntity(YAML::detail::iterator_value& data, Entity& entity)
    {
        // 变换组件数据
        auto transformComponent = data["TransformComponent"];
        if (transformComponent) {
            auto& tc = entity.GetComponent<TransformComponent>();
            tc.Translation = transformComponent["Translation"].as<glm::vec3>();
            tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
            tc.Scale = transformComponent["Scale"].as<glm::vec3>();
        }

        // 相机组件数据
        auto cameraComponent = data["CameraComponent"];
        if (cameraComponent) {
            auto& cameraProps = cameraComponent["Camera"];
            auto& cc = entity.AddComponent<CameraComponent>();

            cc.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());
            cc.Camera.SetPerspectiveVerticalFOV(cameraProps["PerspectiveFOV"].as<float>());
            cc.Camera.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
            cc.Camera.SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());
            cc.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
            cc.Camera.SetOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
            cc.Camera.SetOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());
            // Unlike Camera, Primary is a separate key-value mapping, 
            // while Camera is a map that requires further access.
            cc.Primary = cameraComponent["Primary"].as<bool>();
            cc.FixedAspectRatio = cameraComponent["Fixed Aspect Ratio"].as<bool>();
        }

        // 精灵组件数据
        auto spriteComponent = data["SpriteComponent"];
        if (spriteComponent) {
            auto& sc = entity.AddComponent<SpriteComponent>();
            sc.Color = spriteComponent["Color"].as<glm::vec4>();
        }
    }

    
}
