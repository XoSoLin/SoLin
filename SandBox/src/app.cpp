#include<SoLin.h>

#include "imgui/imgui.h"

class ExampleLayer :public SoLin::Layer {
private:
	std::shared_ptr<SoLin::Shader> m_Shader;
	std::shared_ptr<SoLin::VertexArray> m_VertexArray;

	std::shared_ptr<SoLin::Shader> m_SquareShader;
	std::shared_ptr<SoLin::VertexArray> m_SquareVA;

	SoLin::OrthoGraphicCamera m_Camera;
public:
	ExampleLayer()
		:Layer("Example layer"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 1.0f, 0.8f, 0.2f, 1.0f
		};

		SoLin::BufferLayout layout = {
			{SoLin::ShaderDataType::Float3,"a_Position"},
			{ SoLin::ShaderDataType::Float4, "a_Color" }
		};

		unsigned int indices[3]{
			0,1,2
		};

		std::shared_ptr<SoLin::VertexBuffer> vertexBuffer;
		std::shared_ptr<SoLin::IndexBuffer> indexBuffer;
		m_VertexArray.reset(SoLin::VertexArray::Create());
		vertexBuffer.reset(SoLin::VertexBuffer::Create(vertices, sizeof(vertices)));
		indexBuffer.reset(SoLin::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		vertexBuffer->SetLayout(layout);

		m_VertexArray->AddVertexBuffer(vertexBuffer);
		m_VertexArray->SetIndexBuffer(indexBuffer);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			out vec3 v_Position;
			out vec4 v_Color;

			uniform mat4 u_ViewProjection;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}
		)";
		std::string fragmentSrc = R"(
			#version 330 core

			in vec3 v_Position;
			in vec4 v_Color;
			layout(location = 0) out vec4 a_Color;
			
			void main()
			{
				a_Color = vec4(v_Position * 0.5 + 0.5, 1.0);
				a_Color = v_Color;
			}
		)";
		// 以上对a_Color的操作：使其颜色与位置关联(x轴与r关联，y轴与g关联)
		//						位置是-1到1之间，*0.5使其区间与RGBA的区间大小一致，再+0.5使其区间从-0.5至0.5提升到0至1

		m_Shader.reset(new SoLin::Shader(vertexSrc, fragmentSrc));

		// square rendering
		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, -0.1f,
			 0.75f, -0.75f, -0.1f,
			 0.75f,  0.75f, -0.1f,
			-0.75f,  0.75f, -0.1f
		};
		uint32_t squareIndices[6] = { 0,1,2,2,3,0 };
		m_SquareVA.reset(SoLin::VertexArray::Create());
		std::shared_ptr<SoLin::VertexBuffer> squareVB;
		std::shared_ptr<SoLin::IndexBuffer> squareIB;
		squareVB.reset(SoLin::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareIB.reset(SoLin::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));

		SoLin::BufferLayout squareLayout = {
			{SoLin::ShaderDataType::Float3,"a_Position"}
		};
		squareVB->SetLayout(squareLayout);
		m_SquareVA->AddVertexBuffer(squareVB);
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string squareVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;			

			void main()
			{
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}
		)";
		std::string squareFragSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 a_Color;

			void main()
			{
				a_Color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";

		m_SquareShader.reset(new SoLin::Shader(squareVertexSrc, squareFragSrc));
	}

	void OnUpdate() override {
		SoLin::RendererCommand::Clear();
		SoLin::RendererCommand::SetClearColor({ 0.1f,0.1f,0.1f,1 });
		SoLin::Renderer::BeginScene(m_Camera);

		// 注意物体与实际的相机变化相反
		m_Camera.SetRotation(45.0f);  //相机绕z轴旋转45f(应该是弧度制)，z轴朝屏幕内，屏幕上感觉也就是逆时针
		m_Camera.SetPosition({ 0.5f, 0.5f, 0.0f });//相机朝右上移动

		SoLin::Renderer::Submit(m_SquareShader, m_SquareVA);

		SoLin::Renderer::Submit(m_Shader, m_VertexArray);

		SoLin::Renderer::EndScene();
		//SL_INFO("ExampleLayer::OnUpdate");
	} 
	void OnImGuiRender()
	{
		ImGui::Begin("Test"); 
        const char* text = R"(vva)";
        ImGui::Text(text);
		ImGui::End();
	}

	void OnEvent(SoLin::Event& event)override {
		//SL_TRACE("{0}", event.ToString());
		if (event.GetEventType() == SoLin::EventType::KeyPressed) {
			SoLin::KeyPressedEvent& e = (SoLin::KeyPressedEvent&)event;
			if (e.GetKeyCode() == SL_KEY_TAB)
				SL_TRACE("{0} is pressed | Tab", "Tab");
			SL_TRACE("{0} is pressed | (from char)", (char)e.GetKeyCode());
		}
	}
};

class SandBox :public SoLin::Application {
public:
	SandBox() {
		PushLayer(new ExampleLayer());
		//取消 PushOverlay(new SoLin::ImGuiLayer()); ，将其作为 SoLin 运行时 固定自动添加的图层
	}
	~SandBox() {

	}
};


SoLin::Application* SoLin::CreateApplication() {
	return new SandBox();
}