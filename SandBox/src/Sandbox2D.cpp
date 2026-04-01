#include "Sandbox2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include<chrono>

//----------------------------Timer---------------------------------

struct ProfileResult {
	const char* Name;
	float Time;
};
std::vector<ProfileResult> m_ProfileResults;

template<typename Fn>
class Timer {
public:
	Timer(const char* name,Fn&& func)
		:m_Name(name), m_Func(func),m_Stopped(false) {
		m_StartTimePoint = std::chrono::high_resolution_clock::now();
	}
	~Timer() {
		if (!m_Stopped) {
			Stop();
		}
	}
	void Stop() {
		auto endTimePoint = std::chrono::high_resolution_clock::now();
		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimePoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

		m_Stopped = true;
		float duration = (end - start) * 0.001f;
		m_Func({ m_Name,duration });
	}

private:
	const char* m_Name;
	Fn m_Func;
	bool m_Stopped;
	std::chrono::time_point<std::chrono::steady_clock> m_StartTimePoint;
};

#define PROFILE_SCOPE(name) Timer timer_##__LINE__(name,[&](ProfileResult profileResult){m_ProfileResults.push_back(profileResult);})

//---------------------------------------------------------------------------------

Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"),m_CameraController(1280.0f/720.0f,true)
{
}

void Sandbox2D::OnAttach()
{
	//文件编码转utf-8后在vs中运行时utf-8会被当作gb2312去处理，有中文时会导致程序运行时有乱码
	m_Texture = SoLin::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(SoLin::Timestep ts)
{
	PROFILE_SCOPE("Sandbox2D::OnUpdate");
	//Update
	{
		PROFILE_SCOPE("CameraController::OnUpdate");
		m_CameraController.OnUpdate(ts);
	}
	//Render
	{
		PROFILE_SCOPE("RenderCommand Prep");
		SoLin::RendererCommand::SetClearColor({ 0.1f,0.1f,0.1f,1 });
		SoLin::RendererCommand::Clear();
	}
		
	{
		PROFILE_SCOPE("Renderer2D Draw");
		//这里会消耗很多时间,改了垂直同步后快了，
		// 测试后应该是设置统一变量影响到这里的，是与OpenGL帧率控制相关
		SoLin::Renderer2D::BeginScene(m_CameraController.GetCamera());
	}
		SoLin::Renderer2D::DrawQuad({ 0.0f,0.0f }, { 1.0f, 1.0f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		SoLin::Renderer2D::DrawQuad({ 1.0f, -1.0f,-0.2 }, { 0.5f, 1.0f }, m_SquareColor);
		//通过opengl设置后，纹理坐标超出-1到1的部分，会重复显示
		SoLin::Renderer2D::DrawQuad({ 0.0f,0.0f,-0.1f }, { 2.0f,2.0f }, m_Texture);
		SoLin::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("SandboxTest");
	ImGui::ColorEdit4("Square Color Edit", glm::value_ptr(m_SquareColor));

	for (auto& result : m_ProfileResults) {
		char label[50];
		strcpy(label, "%.3fms | ");
		strcat(label, result.Name);
		ImGui::Text(label, result.Time);
	}
	m_ProfileResults.clear();

	ImGui::End();
}

void Sandbox2D::OnEvent(SoLin::Event& event)
{
	m_CameraController.OnEvent(event);
}
