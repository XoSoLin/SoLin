#pragma once
#include"entt.hpp"
#include"SoLin/Core/Timestep.h"

namespace SoLin {
    class Scene {
    public:
        Scene();
        ~Scene();

        void OnUpdate(Timestep ts);

        // @brief 创建实体
        entt::entity CreateEntity();

        // @brief 返回注册表
        entt::registry& Reg() { return m_Registry; }
    private:
        entt::registry m_Registry;
    };
}
