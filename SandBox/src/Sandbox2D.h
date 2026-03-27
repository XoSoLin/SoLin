#pragma once

#include "SoLin.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

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
	SoLin::Ref<SoLin::Shader> m_SquareShader;
	SoLin::Ref<SoLin::VertexArray> m_SquareVA;
	SoLin::OrthoGraphicCameraController m_CameraController;

	glm::vec4 m_SquareColor = { 0.5412f, 0.1686f, 0.8863f, 1.0f };
};