#include"slpch.h"
#include"Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace SoLin {

	Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();

	void Renderer::Init()
	{
		RendererCommand::Init();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RendererCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(OrthoGraphicCamera& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}
	void Renderer::EndScene()
	{
	}
	void Renderer::Submit(const Ref<Shader>& shader,const Ref<VertexArray>& vertexArray, const glm::mat4& transform )
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UpdateUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UpdateUniformMat4("u_Transform", transform);
		vertexArray->Bind();
		RendererCommand::DrawIndexed(vertexArray);
	}
}