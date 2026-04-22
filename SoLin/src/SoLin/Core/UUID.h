#pragma once
#include <xhash>

namespace SoLin {
    class UUID {
    public:
        UUID();
        UUID(uint64_t uuid);
        UUID(const UUID&) = default;

        operator uint64_t() const { return m_UUID; }
    private:
        uint64_t m_UUID;
    };
}

namespace std {
    // 为 UUID 类型定义哈希函数 确保UUID可以被哈希处理
    template<>
    struct hash<SoLin::UUID> {
        std::size_t operator()(const SoLin::UUID& uuid)const {
            return hash<uint64_t>()((uint64_t)uuid);
        }
    };
}
