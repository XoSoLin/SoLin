#include "slpch.h"
#include"Renderer2D.h"

#include "glm/gtc/matrix_transform.hpp"

#include"Shader.h"
#include"VertexArray.h"
#include"Texture.h"
#include"RendererCommand.h"
#include"Platform/OpenGL/OpenGLShader.h"

namespace SoLin {

    // @brief 四边形顶点结构
    struct QuadVertex {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;

        float TexIndex;
        float TilingFactor;
    };

	//@brief 2D渲染存储
	struct Renderer2DData {
        static const uint32_t MaxTextureSlots = 32;
        const uint32_t MaxQuads = 100;
        const uint32_t MaxVertices = MaxQuads * 4;
        const uint32_t MaxIndices = MaxQuads * 6;

		Ref<VertexArray> QuadVA;		//方形顶点数组指针
        Ref<VertexBuffer> QuadVB;       //方形顶点缓冲区指针
        Ref<IndexBuffer> QuadIB;        //方形索引缓冲区指针
		Ref<Shader> TextureShader;		//纹理着色器指针
		Ref<Texture2D> WhiteTexture;	//1x1的白色纹理(与纹理着色器结合使用达到纯色着色器的效果)

        uint32_t QuadIndexCount = 0;
        QuadVertex* QuadVBBase = nullptr;       //顶点指针起始
        QuadVertex* QuadVBHind = nullptr;       //顶点指针末尾

        std::array<Ref<Texture2D>, MaxTextureSlots> Textures;
        //从1开始，0是白色纹理用来扩展纯色渲染
        uint32_t TextureSoltIndex = 1;
	};
	static Renderer2DData s_Data;	    //2D渲染器数据(批渲染改为栈上分配)


	void Renderer2D::Init()
	{
        SL_PROFILE_FUNCTION();

        //-------------VB----------------
        //GPU创建顶点缓冲区(暂无数据)
        s_Data.QuadVB = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		//设置布局后设置
		BufferLayout squareLayout = {
			{ShaderDataType::Float3,"a_Position"},
            {ShaderDataType::Float4,"a_Color"},
			{ShaderDataType::Float2,"a_TexCoord"},
            {ShaderDataType::Float, "a_TexIndex"},
            {ShaderDataType::Float, "a_TilingFactor"}
		};
		s_Data.QuadVB->SetLayout(squareLayout);

		//-------------IB----------------
        //CPU在堆上设置索引数据
        uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];
        uint32_t offset = 0;
        for (uint32_t i = 0;i < s_Data.MaxIndices;i += 6) {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;

            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;

            // 顶点缓冲区偏移
            offset += 4;
        }
        //将索引数据创建在GPU缓冲区后删除堆上的数据
        s_Data.QuadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
        delete[] quadIndices;


        //-------------VA----------------
		s_Data.QuadVA = VertexArray::Create();
		s_Data.QuadVA->AddVertexBuffer(s_Data.QuadVB);
		s_Data.QuadVA->SetIndexBuffer(s_Data.QuadIB);

        // CPU在堆上创建VB数据，并保存VB指针初始位置
        s_Data.QuadVBBase = new QuadVertex[s_Data.MaxVertices];

        // 创建采样器
        int32_t samplers[s_Data.MaxTextureSlots];
        for (uint32_t i = 0;i < s_Data.MaxTextureSlots;i++)
            samplers[i] = i;

        //-----------Shader--------------
		s_Data.TextureShader = Shader::Create("assets/shaders/TextureShader.glsl");
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetIntArray("u_Textures",samplers,s_Data.MaxTextureSlots);

        //-----------Texture-------------
		//创建单像素白色纹理
		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
        s_Data.Textures[0] = s_Data.WhiteTexture;
	}

	void Renderer2D::Shutdown()
	{
        SL_PROFILE_FUNCTION();

	}

	void Renderer2D::BeginScene(const OrthoGraphicCamera& camera)
	{
        SL_PROFILE_FUNCTION();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

        // 批渲染重置计数和尾指针
        s_Data.QuadIndexCount = 0;
        s_Data.TextureSoltIndex = 1;
        s_Data.QuadVBHind = s_Data.QuadVBBase;
	}

	void Renderer2D::EndScene()
	{
        SL_PROFILE_FUNCTION();

        uint32_t dataSize = (uint8_t*)s_Data.QuadVBHind - (uint8_t*)s_Data.QuadVBBase;
        //将堆上的数据更新至GPU的顶点缓冲区
        s_Data.QuadVB->SetData(s_Data.QuadVBBase, dataSize);

        // 更新数据之后绘制（刷新）
        Flush();
	}

    void Renderer2D::Flush()
    {
        SL_PROFILE_FUNCTION();

        //绑定纹理
        for (uint32_t i = 0;i < s_Data.TextureSoltIndex;i++)
            s_Data.Textures[i]->Bind(i);

        //进行批渲染绘制
        RendererCommand::DrawIndexed(s_Data.QuadVA, s_Data.QuadIndexCount);
    }

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x,position.y,0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
        SL_PROFILE_FUNCTION();

        const float textureIndex = 0.0f;
        const float tilingFactor = 1.0f;

        // 左下(目前锚点默认为此处)
        s_Data.QuadVBHind->Position = position;
        s_Data.QuadVBHind->Color = color;
        s_Data.QuadVBHind->TexCoord = { 0.0f,0.0f };
        s_Data.QuadVBHind->TexIndex = textureIndex;
        s_Data.QuadVBHind->TilingFactor = tilingFactor;
        s_Data.QuadVBHind++;

        //右下
        s_Data.QuadVBHind->Position = { position.x + size.x, position.y, 0.0f };
        s_Data.QuadVBHind->Color = color;
        s_Data.QuadVBHind->TexCoord = { 1.0f, 0.0f };
        s_Data.QuadVBHind->TexIndex = textureIndex;
        s_Data.QuadVBHind->TilingFactor = tilingFactor;
        s_Data.QuadVBHind++;

        //右上
        s_Data.QuadVBHind->Position = { position.x + size.x, position.y + size.y, 0.0f };
        s_Data.QuadVBHind->Color = color;
        s_Data.QuadVBHind->TexCoord = { 1.0f, 1.0f };
        s_Data.QuadVBHind->TexIndex = textureIndex;
        s_Data.QuadVBHind->TilingFactor = tilingFactor;
        s_Data.QuadVBHind++;

        //左上
        s_Data.QuadVBHind->Position = { position.x,			position.y + size.y, 0.0f };
        s_Data.QuadVBHind->Color = color;
        s_Data.QuadVBHind->TexCoord = { 0.0f, 1.0f };
        s_Data.QuadVBHind->TexIndex = textureIndex;
        s_Data.QuadVBHind->TilingFactor = tilingFactor;
        s_Data.QuadVBHind++;

        s_Data.QuadIndexCount += 6;

#if 0
		glm::mat4 transform = glm::translate(
            glm::mat4(1.0f), position) *
            glm::scale(glm::mat4(1.0f),{ size.x,size.y,1.0f }
        );

		s_Data.TextureShader->SetFloat4("u_Color", color);
		s_Data.TextureShader->SetMat4("u_Transform", transform);
        s_Data.TextureShader->SetFloat("u_TilingFactor",1.0f);

		//使用白色像素纹理与color逐分量相乘，不影响传入的color颜色（纯色四边形关键）
		s_Data.WhiteTexture->Bind();

		s_Data.QuadVA->Bind();
		RendererCommand::DrawIndexed(s_Data.QuadVA);
#endif
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture,float tilingFactor,const glm::vec4& tintColor)
	{
		DrawQuad({ position.x,position.y,0.0f }, size, texture,tilingFactor,tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture,float tilingFactor,const glm::vec4& tintColor)
	{
        SL_PROFILE_FUNCTION();

        const glm::vec4 color = tintColor;

        //寻找纹理指针在数组中的位置
        float textureIndex = 0.0f;
        for (uint32_t i = 1;i < s_Data.TextureSoltIndex;i++) {
            if (*s_Data.Textures[i].get() == *texture.get()) {
                textureIndex = (float)i;
                break;
            }
        }
        //纹理指针未命中就添加入数组中
        if (textureIndex == 0.0f) {
            s_Data.Textures[s_Data.TextureSoltIndex] = texture;
            textureIndex = float(s_Data.TextureSoltIndex);
            s_Data.TextureSoltIndex++;
        }

        // 左下(目前锚点默认为此处)
        s_Data.QuadVBHind->Position = position;
        s_Data.QuadVBHind->Color = color;
        s_Data.QuadVBHind->TexCoord = { 0.0f,0.0f };
        s_Data.QuadVBHind->TexIndex = textureIndex;
        s_Data.QuadVBHind->TilingFactor = tilingFactor;
        s_Data.QuadVBHind++;

        //右下
        s_Data.QuadVBHind->Position = { position.x + size.x, position.y, 0.0f };
        s_Data.QuadVBHind->Color = color;
        s_Data.QuadVBHind->TexCoord = { 1.0f, 0.0f };
        s_Data.QuadVBHind->TexIndex = textureIndex;
        s_Data.QuadVBHind->TilingFactor = tilingFactor;
        s_Data.QuadVBHind++;

        //右上
        s_Data.QuadVBHind->Position = { position.x + size.x, position.y + size.y, 0.0f };
        s_Data.QuadVBHind->Color = color;
        s_Data.QuadVBHind->TexCoord = { 1.0f, 1.0f };
        s_Data.QuadVBHind->TexIndex = textureIndex;
        s_Data.QuadVBHind->TilingFactor = tilingFactor;
        s_Data.QuadVBHind++;

        //左上
        s_Data.QuadVBHind->Position = { position.x,			position.y + size.y, 0.0f };
        s_Data.QuadVBHind->Color = color;
        s_Data.QuadVBHind->TexCoord = { 0.0f, 1.0f };
        s_Data.QuadVBHind->TexIndex = textureIndex;
        s_Data.QuadVBHind->TilingFactor = tilingFactor;
        s_Data.QuadVBHind++;

        s_Data.QuadIndexCount += 6;

        #if 0
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });

		// 使用全1的RGBA不影响纹理的取色
		s_Data.TextureShader->SetFloat4("u_Color", tintColor);
		s_Data.TextureShader->SetMat4("u_Transform", transform);
        s_Data.TextureShader->SetFloat("u_TilingFactor", tilingFactor);

		texture->Bind();

		s_Data.QuadVA->Bind();
		RendererCommand::DrawIndexed(s_Data.QuadVA);
        #endif
	}

    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
    {
        DrawRotatedQuad({ position.x,position.y,0.0f }, size, rotation, color);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
    {
        SL_PROFILE_FUNCTION();

        glm::mat4 transform =
            glm::translate(glm::mat4(1.0f), position)
            * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f))
            * glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f }
        );

        s_Data.TextureShader->SetFloat4("u_Color", color);
        s_Data.TextureShader->SetMat4("u_Transform", transform);
        s_Data.TextureShader->SetFloat("u_TilingFactor", 1.0f);

        //使用白色像素纹理与color逐分量相乘，不影响传入的color颜色（纯色四边形关键）
        s_Data.WhiteTexture->Bind();

        s_Data.QuadVA->Bind();
        RendererCommand::DrawIndexed(s_Data.QuadVA);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
    {
        DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
    {
        SL_PROFILE_FUNCTION();

        glm::mat4 transform =
            glm::translate(glm::mat4(1.0f), position)
            * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f))
            * glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f }
            );

        // 使用全1的RGBA不影响纹理的取色
        s_Data.TextureShader->SetFloat4("u_Color", tintColor);
        s_Data.TextureShader->SetMat4("u_Transform", transform);
        s_Data.TextureShader->SetFloat("u_TilingFactor", tilingFactor);

        texture->Bind();

        s_Data.QuadVA->Bind();
        RendererCommand::DrawIndexed(s_Data.QuadVA);
    }

}
