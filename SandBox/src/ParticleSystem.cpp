#include"ParticleSystem.h"

#include<glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

//------------------------------------------------------

#include<random>

class Random {
public:
    static void Init() {
        // 利用种子初始化引擎
        s_RandomEngine.seed(std::random_device()());
    }
    static float Float()
    {
        // 生成随机数，控制在[0,1]之间
        return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
    }
private:
    // 随机数引擎
    static std::mt19937 s_RandomEngine;
    // 随机数分发器
    static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
};

// 定义
std::mt19937 Random::s_RandomEngine;
std::uniform_int_distribution<std::mt19937::result_type> Random::s_Distribution;


// ----------------------------------------------------------

ParticleSystem::ParticleSystem(uint32_t maxParticles)
    :m_PoolIndex(maxParticles-1)
{
    //提前扩充，避免后续的自动扩充
    m_ParticlePool.resize(maxParticles);
}

void ParticleSystem::OnUpdate(SoLin::Timestep ts)
{
    for (auto& particle : m_ParticlePool) {
        if (!particle.Active)
            continue;
        if (particle.LifeRemaining <= 0.0f) {
            particle.Active = false;
            continue;
        }

        // 更新粒子数据
        particle.LifeRemaining -= ts;
        particle.Position += particle.Velocity * (float)ts;
        particle.Rotation += 0.01f * ts;
    }
}

void ParticleSystem::OnRender(SoLin::OrthoGraphicCamera& camera)
{
    SoLin::Renderer2D::BeginScene(camera);

    for (auto& particle : m_ParticlePool) {
        if (!particle.Active)
            continue;

        // 粒子插值渐变的数据
        float life = particle.LifeRemaining / particle.LifeTime;
        glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);
        float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);

        SoLin::Renderer2D::DrawRotatedQuad(
            { particle.Position.x,particle.Position.y,0.4f },
            { size,size }, particle.Rotation, color
        );
    }

    SoLin::Renderer2D::EndScene();
}

void ParticleSystem::Emit(const ParticleProps& particleProps)
{
    Particle& particle = m_ParticlePool[m_PoolIndex];
    particle.Active = true;
    particle.Position = particleProps.Position;
    particle.Rotation = Random::Float() * 2.0f * glm::pi<float>();      //[0,2pi]随机值

    // Velocity
    particle.Velocity = particleProps.Velocity;
    // 为速度给予随机变化
    particle.Velocity.x += particleProps.VelocityVariation.x * (Random::Float() - 0.5f);
    particle.Velocity.y += particleProps.VelocityVariation.y * (Random::Float() - 0.5f);

    // Color
    particle.ColorBegin = particleProps.ColorBegin;
    particle.ColorEnd = particleProps.ColorEnd;

    particle.LifeTime = particleProps.LifeTime;
    particle.LifeRemaining = particleProps.LifeTime;
    // 为初始尺寸给予随机变化
    particle.SizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (Random::Float() - 0.5f);
    particle.SizeEnd = particleProps.SizeEnd;

    m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
}
