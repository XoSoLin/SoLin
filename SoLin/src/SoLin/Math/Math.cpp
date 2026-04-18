#include "slpch.h"
#include "Math.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

namespace SoLin::Math {
    bool DecomposeTransform(const glm::mat4& transform, glm::vec3& outTranslation, glm::vec3& outRotation, glm::vec3& outScale)
    {
        using namespace glm;
        using T = float;

        mat4 LocalMatrix(transform);

        // 检查矩阵的 [3][3] 元素（齐次坐标的 w 分量）是否为 0。如果为 0，说明矩阵是奇异的（不可逆），分解失败。
        if (epsilonEqual(LocalMatrix[3][3], static_cast<float>(0), epsilon<T>()))
            return false;

        // 检查最后一列的前三个元素（[0][3], [1][3], [2][3]）是否为非零。
        // 这些值通常表示透视投影的平移分量。
        // 如果存在，说明矩阵包含透视信息，代码将其清零，只保留仿射变换部分。
        // 因为该函数只处理仿射变换（平移、旋转、缩放）。
        if (
            epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
            epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
            epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>()))
        {
            // Clear the perspective partition
            LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
            LocalMatrix[3][3] = static_cast<T>(1);
        }

        // 在矩阵的第4行获取位置信息，要保留w分量用于后续的计算
        outTranslation = vec3(LocalMatrix[3]);
        LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);


        vec3 Row[3], Pdum3;

        // 将 3x3 旋转缩放部分复制到 Row 数组。
        for (length_t i = 0; i < 3; ++i)
            for (length_t j = 0; j < 3; ++j)
                Row[i][j] = LocalMatrix[i][j];

        // 分别计算每行模长来获取对应轴的缩放，再进行归一化
        outScale.x = length(Row[0]);
        Row[0] = detail::scale(Row[0], static_cast<T>(1));
        outScale.y = length(Row[1]);
        Row[1] = detail::scale(Row[1], static_cast<T>(1));
        outScale.z = length(Row[2]);
        Row[2] = detail::scale(Row[2], static_cast<T>(1));

        // 检测矩阵是否包含镜像（反射），如果行列式为负，则将缩放因子取反来修正。
#if 0
        Pdum3 = cross(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
        if (dot(Row[0], Pdum3) < 0)
        {
            for (length_t i = 0; i < 3; i++)
            {
                scale[i] *= static_cast<T>(-1);
                Row[i] *= static_cast<T>(-1);
            }
        }
#endif

        outRotation.y = asin(-Row[0][2]);
        // 从归一化后的旋转矩阵提取欧拉角（顺序：Y轴 → X轴 → Z轴，即 YXZ 顺序）。
        // Pitch (Y) = asin(-R[0][2])
        // Roll(X) = atan2(R[1][2], R[2][2])
        // Yaw(Z) = atan2(R[0][1], R[0][0])
        // 万向锁处理：当 Pitch = ±90° 时（cos(Pitch) = 0），Yaw 和 Roll 无法唯一确定，只能固定其中一个为 0，计算另一个。
        if (cos(outRotation.y) != 0) {
            outRotation.x = atan2(Row[1][2], Row[2][2]);
            outRotation.z = atan2(Row[0][1], Row[0][0]);
        }
        else {
            outRotation.x = atan2(-Row[2][0], Row[1][1]);
            outRotation.z = 0;
        }


        return true;
    }
}
