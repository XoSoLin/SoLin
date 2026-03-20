#include<SoLin.h>

#include"Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"

class ExampleLayer :public SoLin::Layer {
public:
	ExampleLayer()
		:Layer("Example layer"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f), m_CameraRotation(0.0f)
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
			uniform mat4 u_Transform;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
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

		m_Shader.reset(SoLin::Shader::Create(vertexSrc, fragmentSrc));

		// square rendering
		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, -0.1f,
			 0.5f, -0.5f, -0.1f,
			 0.5f,  0.5f, -0.1f,
			-0.5f,  0.5f, -0.1f
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
			uniform mat4 u_Transform;		

			void main()
			{
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";
		std::string squareFragSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 a_Color;
			
			uniform vec3 u_Color;
			
			void main()
			{
				a_Color = vec4(u_Color, 1.0);
			}
		)";

		m_SquareShader.reset(SoLin::Shader::Create(squareVertexSrc, squareFragSrc));
	}

	virtual void OnUpdate(SoLin::Timestep& ts) override {
		
		//控制相机在自己坐标系中移动旋转

		if (SoLin::Input::IsKeyPressed(SL_KEY_RIGHT))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;
		else if (SoLin::Input::IsKeyPressed(SL_KEY_LEFT))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;

		if (SoLin::Input::IsKeyPressed(SL_KEY_UP))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		else if (SoLin::Input::IsKeyPressed(SL_KEY_DOWN))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;

		if (SoLin::Input::IsKeyPressed(SL_KEY_Q))
			m_CameraRotation += m_CameraRotateSpeed * ts;
		else if (SoLin::Input::IsKeyPressed(SL_KEY_E))
			m_CameraRotation -= m_CameraRotateSpeed * ts;

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		SoLin::RendererCommand::Clear();
		SoLin::RendererCommand::SetClearColor({ 0.1f,0.1f,0.1f,1 });
		SoLin::Renderer::BeginScene(m_Camera);

		// 缩放变换
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		m_SquareShader->Bind();
		std::dynamic_pointer_cast<SoLin::OpenGLShader>(m_SquareShader)->UpdateUniformFloat3("u_Color", m_SquareColor);
		// 双层循环渲染400个正方形
		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				// 计算每一个的位置   使其之间有些间隔
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				// 进行 TS 变换(先缩放S，再平移T)
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				
				SoLin::Renderer::Submit(m_SquareShader, m_SquareVA,transform);
			}
		}


		SoLin::Renderer::Submit(m_Shader, m_VertexArray,glm::mat4(1.0f));

		SoLin::Renderer::EndScene();
		//SL_INFO("ExampleLayer::OnUpdate");
	} 
	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Test"); 
		ImGui::ColorEdit3("Square Color Edit", glm::value_ptr(m_SquareColor));
        const char* text = R"(vva)";
        ImGui::Text(text);
		ImGui::End();
	}

	virtual void OnEvent(SoLin::Event& event)override {
		//SL_TRACE("{0}", event.ToString());
		if (event.GetEventType() == SoLin::EventType::KeyPressed) {
			SoLin::KeyPressedEvent& e = (SoLin::KeyPressedEvent&)event;
			if (e.GetKeyCode() == SL_KEY_TAB)
				SL_TRACE("{0} is pressed | Tab", "Tab");
			SL_TRACE("{0} is pressed | (from char)", (char)e.GetKeyCode());
		}
	}

	private:
		std::shared_ptr<SoLin::Shader> m_Shader;
		std::shared_ptr<SoLin::VertexArray> m_VertexArray;

		std::shared_ptr<SoLin::Shader> m_SquareShader;
		std::shared_ptr<SoLin::VertexArray> m_SquareVA;

		glm::vec3 m_SquareColor = { 0.5412f, 0.1686f, 0.8863f };

		SoLin::OrthoGraphicCamera m_Camera;

		glm::vec3 m_CameraPosition;
		float m_CameraMoveSpeed = 4.0f;
		float m_CameraRotation;
		float m_CameraRotateSpeed = 180.0f;
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