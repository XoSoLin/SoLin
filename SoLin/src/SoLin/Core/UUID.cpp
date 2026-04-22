#include "slpch.h"
#include "SoLin/Core/UUID.h"

#include<random>

namespace SoLin {
    static std::random_device s_RD;             //设置随机种子
    static std::mt19937_64 s_Engine(s_RD());    // 为引擎设置种子
    // 创建分布对象 用于约束随机数生成的范围、频率等特征
    static std::uniform_int_distribution<uint64_t> s_UniforDistribution;


    UUID::UUID()
        :m_UUID(s_UniforDistribution(s_Engine))
    {
    }

    UUID::UUID(uint64_t uuid)
        :m_UUID(uuid)
    {
    }

}
