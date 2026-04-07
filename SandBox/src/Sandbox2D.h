#pragma once

#include "SoLin.h"

#include "imgui/imgui.h"
#include"SoLin/Renderer/Texture.h"
#include "ParticleSystem.h"

class Sandbox2D :public SoLin::Layer {
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach()override;

	virtual void OnUpdate(SoLin::Timestep ts) override;
	virtual void OnImGuiRender()override;
	virtual void OnEvent(SoLin::Event& event)override;
private:
	SoLin::OrthoGraphicCameraController m_CameraController;
	SoLin::Ref<SoLin::Texture2D> m_Texture;
    SoLin::Ref<SoLin::Texture2D> m_SpriteSheet;
    SoLin::Ref<SoLin::Texture2D> m_SpriteSheetRole;
    SoLin::Ref<SoLin::SubTexture2D> m_Role;

    std::unordered_map<char, SoLin::Ref<SoLin::SubTexture2D>> m_TilesMap;

	glm::vec4 m_SquareColor = { 0.5412f, 0.1686f, 0.8863f, 1.0f };
    ParticleProps m_Particle;
    ParticleSystem m_ParticleSystem;
};
