#include "slpch.h"
#include"Renderer2D.h"

#include "glm/gtc/matrix_transform.hpp"

#include"Shader.h"
#include"VertexArray.h"
#include"Texture.h"
#include"RendererCommand.h"
#include"Platform/OpenGL/OpenGLShader.h"

namespace SoLin {

	//@brief 2D渲染存储
	struct Renderer2DStorage {
		Ref<VertexArray> QuadVA;		//方形顶点数组指针
		Ref<Shader> TextureShader;		//纹理着色器指针
		Ref<Texture2D> WhiteTexture;	//1x1的白色纹理(与纹理着色器结合使用达到纯色着色器的效果)
	};
	static Renderer2DStorage* s_Data;	//2D渲染器数据


	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage();

		//顶点数据
		float squareVertices[5 * 4] = {
			-0.5f,-0.5f,-0.1f, 0.0f, 0.0f,
			0.5f ,-0.5f,-0.1f, 1.0f, 0.0f,
			0.5f ,0.5f ,-0.1f, 1.0f, 1.0f,
			-0.5f,0.5f ,-0.1f, 0.0f, 1.0f
		};
		//索引数据
		uint32_t squareIndices[6] = { 0,1,2,2,3,0 };

		// 创建缓冲区
		Ref<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		Ref<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));

		//设置布局
		BufferLayout squareLayout = {
			{ShaderDataType::Float3,"a_Position"},
			{ShaderDataType::Float2,"a_TexCoord"}
		};
		squareVB->SetLayout(squareLayout);

		s_Data->QuadVA = VertexArray::Create();
		s_Data->QuadVA->AddVertexBuffer(squareVB);
		s_Data->QuadVA->SetIndexBuffer(squareIB);


		s_Data->TextureShader = Shader::Create("assets/shaders/TextureShader.glsl");
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("u_Texture", 0);

		//创建单像素白色纹理
		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthoGraphicCamera& camera)
	{
		
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x,position.y,0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });

		s_Data->TextureShader->SetFloat4("u_Color", color);
		s_Data->TextureShader->SetMat4("u_Transform", transform);

		//使用白色像素纹理与color逐分量相乘，不影响传入的color颜色（纯色四边形关键）
		s_Data->WhiteTexture->Bind();

		s_Data->QuadVA->Bind();
		RendererCommand::DrawIndexed(s_Data->QuadVA);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		DrawQuad({ position.x,position.y,0.0f }, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });

		// 使用全1的RGBA不影响纹理的取色
		s_Data->TextureShader->SetFloat4("u_Color", glm::vec4{ 1.0f });
		s_Data->TextureShader->SetMat4("u_Transform", transform);

		texture->Bind();

		s_Data->QuadVA->Bind();
		RendererCommand::DrawIndexed(s_Data->QuadVA);
	}

}