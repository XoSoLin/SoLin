#include"slpch.h"
#include"Entity.h"

namespace SoLin {

    Entity::Entity(entt::entity entityHandle,Scene* scene)
        :m_EntityHandle(entityHandle),m_Scene(scene){ }

}
