#include "Sandbox2D.h"

Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"),m_CameraController(1280.0f/720.0f,true)
{
}

void Sandbox2D::OnAttach()
{
	float squareVertices[5 * 4] =
	{
		-0.5f, -0.5f, -0.1f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.1f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.1f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.1f, 0.0f, 1.0f
	};
	uint32_t squareIndices[6] = { 0,1,2,2,3,0 };

	SoLin::Ref<SoLin::VertexBuffer> squareVB;
	squareVB.reset(SoLin::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
	SoLin::Ref<SoLin::IndexBuffer> squareIB;
	squareIB.reset(SoLin::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));

	SoLin::BufferLayout squareLayout = {
		{SoLin::ShaderDataType::Float3,"a_Position"},
		{SoLin::ShaderDataType::Float2,"a_TexCoord"}
	};
	squareVB->SetLayout(squareLayout);

	m_SquareVA = SoLin::VertexArray::Create();
	m_SquareVA->AddVertexBuffer(squareVB);
	m_SquareVA->SetIndexBuffer(squareIB);

	m_SquareShader = SoLin::Shader::Create("assets/shaders/FlatColorShader.glsl");

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

	SoLin::Renderer::BeginScene(m_CameraController.GetCamera());

	std::dynamic_pointer_cast<SoLin::OpenGLShader>(m_SquareShader)->Bind();
	std::dynamic_pointer_cast<SoLin::OpenGLShader>(m_SquareShader)->UpdateUniformFloat4("u_Color", m_SquareColor);

	SoLin::Renderer::Submit(m_SquareShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
	SoLin::Renderer::EndScene();
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
