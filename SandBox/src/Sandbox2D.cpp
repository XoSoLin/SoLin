#include "Sandbox2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"),m_CameraController(1280.0f/720.0f,true)
{
}

void Sandbox2D::OnAttach()
{
    SL_PROFILE_FUNCTION();

	//文件编码转utf-8后在vs中运行时utf-8会被当作gb2312去处理，有中文时会导致程序运行时有乱码
	m_Texture = SoLin::Texture2D::Create(SLPATH("assets/textures/千夏02.png"));
}

void Sandbox2D::OnDetach()
{
    SL_PROFILE_FUNCTION();

}

void Sandbox2D::OnUpdate(SoLin::Timestep ts)
{
    SL_PROFILE_FUNCTION();
	//Update
	m_CameraController.OnUpdate(ts);
	//Render
	{
		SL_PROFILE_SCOPE("RenderCommand Prep");
		SoLin::RendererCommand::SetClearColor({ 0.1f,0.1f,0.1f,1 });
		SoLin::RendererCommand::Clear();
	}
		
	{
		SL_PROFILE_SCOPE("Renderer2D Draw");

        static float temp = 0.0f;
        temp += ts * 100.0f;

		//这里会消耗很多时间,改了垂直同步后快了，
		// 测试后应该是设置统一变量影响到这里的，是与OpenGL帧率控制相关
		SoLin::Renderer2D::BeginScene(m_CameraController.GetCamera());
		SoLin::Renderer2D::DrawQuad({ 0.0f,0.0f }, { 1.0f, 1.0f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		SoLin::Renderer2D::DrawQuad({ 1.0f, -1.0f,-0.2 }, { 0.5f, 1.0f }, m_SquareColor);
		//通过opengl设置后，纹理坐标超出-1到1的部分，会重复显示
        SoLin::Renderer2D::DrawRotatedQuad({ -1.0f, 1.0f }, { 1.0f, 1.0f }, temp, { 0.3f, 0.2f, 0.8f, 1.0f });
		SoLin::Renderer2D::DrawQuad({ 0.0f,0.0f,-0.1f }, { 2.0f,2.0f }, m_Texture,10.0f,{1.0,0.9,0.9,1.0});
		SoLin::Renderer2D::EndScene();

        SL::Renderer2D::BeginScene(m_CameraController.GetCamera());
        for (float y = -5.0f; y < 5.0f; y += 0.5f)
        {
            for (float x = -5.0f; x < 5.0f; x += 0.5f)
            {
                glm::vec4 color = { 0.0f ,(x + 5.0f) / 10.0f, (y + 5.0f) / 10.0f, 0.7f };
                SL::Renderer2D::DrawQuad({ x,y }, { 0.45f, 0.45f }, color);
            }
        }
        SL::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
    SL_PROFILE_FUNCTION();

	ImGui::Begin("SandboxTest");
    auto stats = SL::Renderer2D::GetStats();
    ImGui::Text("Renderer2D Stats:");
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quads: %d", stats.QuadCount);
    ImGui::Text("Vertices: %d", stats.GetVertexCount());
    ImGui::Text("Indices: %d", stats.GetIndexCount());
	ImGui::ColorEdit4("Square Color Edit", glm::value_ptr(m_SquareColor));

	ImGui::End();
}

void Sandbox2D::OnEvent(SoLin::Event& event)
{
	m_CameraController.OnEvent(event);
}
