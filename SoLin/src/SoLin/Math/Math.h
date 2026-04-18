#pragma once

#include<glm/glm.hpp>

namespace SoLin {
    namespace Math {
        // @brief 分解变换矩阵
        // @param transform 预分解矩阵
        // @param outTranslation 输出位置向量
        // @param outRotation 输出旋转向量
        // @param outScale 输出缩放向量
        bool DecomposeTransform(const glm::mat4& transform, glm::vec3& outTranslation, glm::vec3& outRotation, glm::vec3& outScale);
    }
}
