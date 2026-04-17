#pragma once
#include "SoLin/Scene/Scene.h"

#include<yaml-cpp/yaml.h>

namespace SoLin {
    class SceneSerializer {
    public:
        SceneSerializer(const Ref<Scene>& scene);

        // @brief 序列化
        void Serialize(const std::string& filepath);
        // @brief 运行时序列化
        void SerializeRuntime(const std::string& filepath);

        // @brief 反序列化
        bool Deserialize(const std::string& filepath);
        // @brief 运行时反序列化
        bool DeserializeRuntime(const std::string& filepath);

    private:
        // @brief 序列化实体
        void SerializeEntity(YAML::Emitter& out, Entity& entity);
        // @brief 反序列化实体
        void DeserializeEntity(YAML::detail::iterator_value& data, Entity& entity);
    private:
        Ref<Scene> m_Scene;
    };
}
