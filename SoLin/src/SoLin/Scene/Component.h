#pragma once

#include"glm/glm.hpp"

namespace SoLin {
    //@brief 变化组件
    struct TransformComponent {
        glm::mat4 Transform{ 1.0f };

        TransformComponent() = default;
        TransformComponent(const glm::mat4& transform)
            :Transform(transform) {
        }
        TransformComponent(const TransformComponent&) = default;				//复制函数

        // 类型转换运算符重载(方便直接以对应类型去使用该类)
        operator glm::mat4& () { return Transform; }
        operator const glm::mat4& () const { return Transform; }
    };

    //@brief 精灵组件
    struct SpriteComponent {
        glm::vec4 Color{ 1.0f,1.0f,1.0f,1.0f };

        SpriteComponent() = default;
        SpriteComponent(const glm::vec4& color)
            :Color(color) {
        };
        SpriteComponent(const SpriteComponent&) = default;

        operator glm::vec4& () { return Color; }
        operator const glm::vec4& ()const { return Color; }
    };


}
