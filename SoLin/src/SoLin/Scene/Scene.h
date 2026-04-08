#pragma once
#include"entt.hpp"

namespace SoLin {
    class Scene {
    public:
        Scene();
        ~Scene();
    private:
        entt::registry m_Registry;
    };
}
