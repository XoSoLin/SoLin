#include "Sandbox2D.h"

Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"),m_CameraController(1280.0f/720.0f,true)
{
}

void Sandbox2D::OnAttach()
{

}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(SoLin::Timestep ts)
{
	//Update
	m_CameraController.OnUpdate(ts);
	//Render
	SoLin::RendererCommand::SetClearColor({ 0.1f,0.1f,0.1f,1 });
	SoLin::RendererCommand::Clear();

	SoLin::Renderer2D::BeginScene(m_CameraController.GetCamera());
	SoLin::Renderer2D::DrawQuad({ 0.0f,0.0f }, { 1.0f, 1.0f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	SoLin::Renderer2D::DrawQuad({ 1.0f, -1.0f }, { 0.5f, 1.0f }, { 0.2f, 0.8f, 0.3f, 1.0f });
	SoLin::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("SandboxTest");
	ImGui::ColorEdit4("Square Color Edit", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(SoLin::Event& event)
{
	m_CameraController.OnEvent(event);
}
