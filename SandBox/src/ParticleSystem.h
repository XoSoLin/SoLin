#pragma once
#include <SoLin.h>

// @brief 粒子属性 数据结构
struct ParticleProps {
    glm::vec2 Position;                     // 位置
    glm::vec2 Velocity;                     // 速度
    glm::vec2 VelocityVariation;            // 初始速度变化幅度
    glm::vec4 ColorBegin, ColorEnd;         // 线性插值色
    float SizeBegin, SizeEnd;               // 线性插值尺寸
    float SizeVariation;                    // 初始尺寸变化幅度
    float LifeTime = 1.0f;                  // 生命周期
};

// @brief 粒子系统
class ParticleSystem {
public:
    // @param maxParticles 最大粒子数(默认1000)
    ParticleSystem(uint32_t maxParticles = 1000);

    void OnUpdate(SoLin::Timestep ts);
    void OnRender(SoLin::OrthoGraphicCamera& camera);

    // @brief 根据粒子属性释放粒子
    // @param particleProps 粒子属性
    void Emit(const ParticleProps& particleProps);
private:
    // @brief 粒子结构
    struct Particle {
        glm::vec2 Position;                 // 位置
        glm::vec2 Velocity;                 // 发射速度
        glm::vec4 ColorBegin, ColorEnd;     // 颜色插值
        float Rotation = 0.0f;              // 旋转角度
        float SizeBegin, SizeEnd;           // 尺寸插值

        float LifeTime = 1.0f;              // 生命周期
        float LifeRemaining = 0.0f;         // 剩余生命

        bool Active = false;                // 激活标志
    };

    std::vector<Particle> m_ParticlePool;       // 粒子池
    uint32_t m_PoolIndex;                       // 池索引
};
