#include "slpch.h"
#include "Application.h"

#include "Events/Event.h"
#include "SoLin/Log.h"
#include"Input.h"

#include"SoLin/Renderer/Renderer.h"

namespace SoLin {

#define BIND_EVENT_FN(x) std::bind(&Application::x,this,std::placeholders::_1)		//绑定一个成员函数，并占位参数
																					//std::bind()用于将一个可调度对象与一些参数绑定在一起，生成一个新的可调度对象,
																					//	其返回的对象有了额外的上下文，不是一个单纯的指针
																					//this 指的就是当前这个对象，是this对象的成员Application::x()的第一个隐藏参数
																					//std::placeholders::_1是传入这个新的可调度对象的一个参数，这里是指 Event&
																					// 以void(int& a,int& b)为例,其为f1   
																					// 当f2 = bind(f1,this,占位符2,占位符1)之后，f1(1,2)就相当于f2(2,1)

	Application* Application::s_Instance = nullptr;									//初始化唯一实例的静态成员s_Instance;

	
	Application::Application()
		:m_Camera(-1.6f,1.6f,-0.9f,0.9f)
	{
		// 如果Appliction再次被构造，将会触发ASSERT，因为第一次构造后将会使s_Instance不再为nullptr
		SL_CORE_ASSERT(!s_Instance, "Application already exists!(The class Application is a Singleton,it just support one instance!)");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());

		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
		/*auto f2 = std::bind(&Application::OnEvent,this,std::placeholders::_1);
		m_Window->SetEventCallback(f2);*/

		m_ImGuiLayer = new ImGuiLayer();												//初始化 m_ImGuiLayer 为原始指针，并推入层栈
		PushOverlay(m_ImGuiLayer);

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 1.0f, 0.8f, 0.2f, 1.0f
		};
		
		BufferLayout layout = {
			{ShaderDataType::Float3,"a_Position"},
			{ ShaderDataType::Float4, "a_Color" }
		};

		unsigned int indices[3]{
			0,1,2
		};

		std::shared_ptr<VertexBuffer> vertexBuffer;
		std::shared_ptr<IndexBuffer> indexBuffer;
		m_VertexArray.reset(VertexArray::Create());
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
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

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));

		// square rendering
		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, -0.1f,
			 0.75f, -0.75f, -0.1f,
			 0.75f,  0.75f, -0.1f,
			-0.75f,  0.75f, -0.1f
		};
		uint32_t squareIndices[6] = { 0,1,2,2,3,0 };
		m_SquareVA.reset(VertexArray::Create());
		std::shared_ptr<VertexBuffer> squareVB;
		std::shared_ptr<IndexBuffer> squareIB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		
		BufferLayout squareLayout = {
			{ShaderDataType::Float3,"a_Position"}
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

		m_SquareShader.reset(new Shader(squareVertexSrc, squareFragSrc));
	}

	Application::~Application()
	{
	}
	
	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay) {
		m_LayerStack.PushOverLay(overlay);
	}

	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		// 这里会输出所有e的信息，但这些事件并不是都被响应处理了。
		// 目前只有WindowCloseEvent被处理，也就是执行了OnWindowClose()
		//SL_CORE_TRACE("{0} | handled({1})", e.ToString(),e.Handled);

		for (auto iter = m_LayerStack.end(); iter != m_LayerStack.begin();) {
			(*--iter)->OnEvent(e);
			if (e.Handled) {
				break;
			}
		}

	}

	void Application::Run() {
		WindowResizeEvent e(1280, 720);
		
		while (m_Running) {
			RendererCommand::Clear();
			RendererCommand::SetClearColor({ 0.1f,0.1f,0.1f,1 });
			Renderer::BeginScene(m_Camera);

			Renderer::Submit(m_SquareShader,m_SquareVA);

			Renderer::Submit(m_Shader,m_VertexArray);

			Renderer::EndScene();

			for (Layer* layer : m_LayerStack) {				//更新图层
				layer->OnUpdate();
			}

			/*auto [x, y] = Input::GetMousePos();
			SL_CORE_TRACE("{0},{1}", x, y);*/

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();							//更新窗口
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& event) {
		m_Running = false;
		return true;
	}
}
