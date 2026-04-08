#pragma once

#include <glm/glm.hpp>

namespace SoLin {

    // @brief 普通相机类
    class Camera {
    public:
        Camera() = default;
        // @param projection 投影矩阵
        Camera(const glm::mat4& projection)
            :m_ProjectionMatrix(projection){ }
        virtual ~Camera() = default;

        const glm::mat4& GetProjection() const { return m_ProjectionMatrix; }
    protected:
        glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);   // 投影矩阵
    };
}
