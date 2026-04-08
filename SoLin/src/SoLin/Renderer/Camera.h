#pragma once

#include <glm/glm.hpp>

namespace SoLin {

    // @brief 普通相机类
    class Camera {
    public:
        // @param projection 投影矩阵
        Camera(const glm::mat4& projection)
            :m_ProjectionMatrix(projection){ }
        const glm::mat4& GetProjection() const { return m_ProjectionMatrix; }
    private:
        glm::mat4 m_ProjectionMatrix;   // 投影矩阵
    };
}
