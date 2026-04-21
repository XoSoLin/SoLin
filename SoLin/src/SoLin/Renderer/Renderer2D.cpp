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

        int EntityID;
    };

	//@brief 2D渲染存储
	struct Renderer2DData {
        static const uint32_t MaxTextureSlots = 32;
        static const uint32_t MaxQuads = 1000;
        static const uint32_t MaxVertices = MaxQuads * 4;
        static const uint32_t MaxIndices = MaxQuads * 6;

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
        uint32_t TextureSlotIndex = 1;

        glm::vec4 QuadVertexPosition[4]{
            { -0.5f, -0.5f, 0.0f, 1.0f },
            {  0.5f, -0.5f, 0.0f, 1.0f },
            {  0.5f,  0.5f, 0.0f, 1.0f },
            { -0.5f,  0.5f, 0.0f, 1.0f }
        };

        Renderer2D::Statistics Stats;
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
            {ShaderDataType::Float, "a_TilingFactor"},
            {ShaderDataType::Int,"a_EntityID"}
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

        delete[] s_Data.QuadVBBase;
	}

    void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& viewMatrix)
    {
        SL_PROFILE_FUNCTION();

        glm::mat4 viewProjectionMatrix = camera.GetProjection() * viewMatrix;

        s_Data.TextureShader->Bind();
        s_Data.TextureShader->SetMat4("u_ViewProjection", viewProjectionMatrix);

        // 批渲染重置计数和尾指针
        s_Data.QuadIndexCount = 0;
        s_Data.TextureSlotIndex = 1;
        s_Data.QuadVBHind = s_Data.QuadVBBase;
    }

    void Renderer2D::BeginScene(const OrthoGraphicCamera& camera)
	{
        SL_PROFILE_FUNCTION();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

        // 批渲染重置计数和尾指针
        s_Data.QuadIndexCount = 0;
        s_Data.TextureSlotIndex = 1;
        s_Data.QuadVBHind = s_Data.QuadVBBase;
	}

    void Renderer2D::BeginScene(const EditorCamera& camera)
    {
        SL_PROFILE_FUNCTION();

        glm::mat4 viewProjectionMatrix = camera.GetViewProjection();

        s_Data.TextureShader->Bind();
        s_Data.TextureShader->SetMat4("u_ViewProjection", viewProjectionMatrix);

        // 批渲染重置计数和尾指针
        s_Data.QuadIndexCount = 0;
        s_Data.TextureSlotIndex = 1;
        s_Data.QuadVBHind = s_Data.QuadVBBase;
    }

	void Renderer2D::EndScene()
	{
        SL_PROFILE_FUNCTION();

        uint32_t dataSize = uint32_t((uint8_t*)s_Data.QuadVBHind - (uint8_t*)s_Data.QuadVBBase);
        //将堆上的数据更新至GPU的顶点缓冲区
        s_Data.QuadVB->SetData(s_Data.QuadVBBase, dataSize);

        // 更新数据之后绘制（刷新）
        Flush();
	}

    void Renderer2D::Flush()
    {
        SL_PROFILE_FUNCTION();

        if (s_Data.QuadIndexCount == 0)
            return;

        //绑定纹理
        for (uint32_t i = 0;i < s_Data.TextureSlotIndex;i++)
            s_Data.Textures[i]->Bind(i);

        //进行批渲染绘制
        RendererCommand::DrawIndexed(s_Data.QuadVA, s_Data.QuadIndexCount);

        s_Data.Stats.DrawCalls++;
    }

    // 如果当前绘制的顶点（或索引）超出了一次批渲染的限制，
    // 便开启下一次批渲染，并清零当前记录的顶点数
    void Renderer2D::FlushAndReset()
    {
        EndScene();

        s_Data.QuadIndexCount = 0;
        s_Data.TextureSlotIndex = 1;
        s_Data.QuadVBHind = s_Data.QuadVBBase;
    }

    void Renderer2D::QuadTransportGLSL(
        const glm::mat4& transform,
        const glm::vec4& color,
        const float& textureIndex,
        const float& tilingFactor,
        const glm::vec2* texcoords,
        const int entityID
    )
    {
        constexpr size_t quadVertexCount = 4;

        for (size_t i = 0;i < quadVertexCount;i++) {
            s_Data.QuadVBHind->Position = transform * s_Data.QuadVertexPosition[i];
            s_Data.QuadVBHind->Color = color;
            s_Data.QuadVBHind->TexCoord = texcoords[i];
            s_Data.QuadVBHind->TexIndex = textureIndex;
            s_Data.QuadVBHind->TilingFactor = tilingFactor;
            s_Data.QuadVBHind->EntityID = entityID;
            s_Data.QuadVBHind++;
        }

        s_Data.QuadIndexCount += 6;
        s_Data.Stats.QuadCount++;
    }

    void Renderer2D::QuadGetTextureIndex(const Ref<Texture2D>& texture,float& index)
    {
        for (uint32_t i = 1;i < s_Data.TextureSlotIndex;i++) {
            if (*s_Data.Textures[i].get() == *texture.get()) {
                index = (float)i;
                break;
            }
        }
        if (index == 0.0f) {
            if (s_Data.TextureSlotIndex >= s_Data.MaxTextureSlots) {
                FlushAndReset();
            }
            s_Data.Textures[s_Data.TextureSlotIndex] = texture;
            index = float(s_Data.TextureSlotIndex);

            s_Data.TextureSlotIndex++;
        }
    }

    void Renderer2D::DrawQuad(const glm::mat4 transform, const glm::vec4 color, const int& entityID)
    {
        SL_PROFILE_FUNCTION();
        if (s_Data.QuadIndexCount >= s_Data.MaxIndices) {
            FlushAndReset();
        }

        constexpr glm::vec2 texCoords[4] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
        constexpr float textureIndex = 0.0f;									
        constexpr float tilingFactor = 1.0f;									

        QuadTransportGLSL(transform, color, textureIndex, tilingFactor, texCoords, entityID);
    }

    void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor, const int& entityID)
    {
        SL_PROFILE_FUNCTION();
        if (s_Data.QuadIndexCount >= s_Data.MaxIndices) {
            FlushAndReset();
        }

        float textureIndex = 0.0f;
        constexpr glm::vec2 texCoords[4] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

        QuadGetTextureIndex(texture, textureIndex);
        QuadTransportGLSL(transform, tintColor, textureIndex, tilingFactor, texCoords, entityID);

    }

    void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D>& subtexture, float tilingFactor, const glm::vec4& tintColor, const int& entityID)
    {
        SL_PROFILE_FUNCTION();
        if (s_Data.QuadIndexCount >= s_Data.MaxIndices) {
            FlushAndReset();
        }

        constexpr size_t quadVertexCount = 4;
        const glm::vec2* subTexCoords = subtexture->GetCoords();
        Ref<Texture2D> texture = subtexture->GetTexture();

        float textureIndex = 0.0f;

        QuadGetTextureIndex(texture, textureIndex);
        QuadTransportGLSL(transform, tintColor, textureIndex, tilingFactor, subTexCoords, entityID);
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x,position.y,0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
        SL_PROFILE_FUNCTION();

        if (s_Data.QuadIndexCount >= s_Data.MaxIndices) {
            FlushAndReset();
        }

        //使用白色像素纹理与color逐分量相乘，不影响传入的color颜色（纯色四边形关键）
        const float textureIndex = 0.0f;
        const float tilingFactor = 1.0f;

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
            * glm::scale(glm::mat4(1.0f), { size.x, size.y, 0.0f });

        constexpr glm::vec2 texCoords[4] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
        QuadTransportGLSL(transform, color, textureIndex, tilingFactor, texCoords);
        
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture,float tilingFactor,const glm::vec4& tintColor)
	{
		DrawQuad({ position.x,position.y,0.0f }, size, texture,tilingFactor,tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture,float tilingFactor,const glm::vec4& tintColor)
	{
        SL_PROFILE_FUNCTION();

        if (s_Data.QuadIndexCount >= s_Data.MaxIndices) {
            FlushAndReset();
        }

        const glm::vec4 color = tintColor;

        //寻找纹理指针在数组中的位置
        float textureIndex = 0.0f;
        for (uint32_t i = 1;i < s_Data.TextureSlotIndex;i++) {
            if (*s_Data.Textures[i].get() == *texture.get()) {
                textureIndex = (float)i;
                break;
            }
        }
        //纹理指针未命中就添加入数组中
        if (textureIndex == 0.0f) {
            // 如果纹理数也多于最大值，就刷新一次批渲染
            if (s_Data.TextureSlotIndex >= s_Data.MaxTextureSlots) {
                FlushAndReset();
            }
            s_Data.Textures[s_Data.TextureSlotIndex] = texture;
            textureIndex = float(s_Data.TextureSlotIndex);
            s_Data.TextureSlotIndex++;
        }

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
            * glm::scale(glm::mat4(1.0f), { size.x, size.y, 0.0f });

        constexpr glm::vec2 texCoords[4] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
        QuadTransportGLSL(transform, color, textureIndex, tilingFactor,texCoords);


	}

    void Renderer2D::DrawQuad(
        const glm::vec2& position, const glm::vec2& size,
        const Ref<SubTexture2D>& subtexture, float tilingFactor,
        const glm::vec4& tintColor) {
        DrawQuad({ position.x, position.y }, size, subtexture, tilingFactor, tintColor);
    }

    void Renderer2D::DrawQuad(
        const glm::vec3& position, const glm::vec2& size,
        const Ref<SubTexture2D>& subtexture, float tilingFactor,
        const glm::vec4& tintColor) {
        SL_PROFILE_FUNCTION();

        if (s_Data.QuadIndexCount >= s_Data.MaxIndices) {
            FlushAndReset();
        }

        constexpr size_t quadVertexCount = 4;
        const glm::vec2* subTexCoords = subtexture->GetCoords();
        Ref<Texture2D>texture = subtexture->GetTexture();

        float textureIndex = 0.0f;

        QuadGetTextureIndex(texture, textureIndex);

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
            * glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });

        QuadTransportGLSL(transform, tintColor, textureIndex, tilingFactor,subTexCoords);

    }

    void Renderer2D::DrawRotatedQuad(const glm::mat4& transform, float rotation, const glm::vec4& color)
    {
        SL_PROFILE_FUNCTION();

        if (s_Data.QuadIndexCount >= s_Data.MaxIndices) {
            FlushAndReset();
        }
        const float textureIndex = 0.0f;
        const float tilingFactor = 1.0f;

        glm::mat4 new_transform = transform
            * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));

        constexpr glm::vec2 texCoords[4] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
        QuadTransportGLSL(new_transform, color, textureIndex, tilingFactor, texCoords);
    }

    void Renderer2D::DrawRotatedQuad(const glm::mat4& transform, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
    {
        SL_PROFILE_FUNCTION();
        if (s_Data.QuadIndexCount >= s_Data.MaxIndices) {
            FlushAndReset();
        }

        float textureIndex = 0.0f;
        constexpr glm::vec2 texCoords[4] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

        glm::mat4 new_transform = transform
            * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));

        QuadGetTextureIndex(texture, textureIndex);
        QuadTransportGLSL(new_transform, tintColor, textureIndex, tilingFactor, texCoords);
    }

    void Renderer2D::DrawRotatedQuad(const glm::mat4& transform, float rotation, const Ref<SubTexture2D>& subtexture, float tilingFactor, const glm::vec4& tintColor) {
        SL_PROFILE_FUNCTION();
        if (s_Data.QuadIndexCount >= s_Data.MaxIndices) {
            FlushAndReset();
        }

        constexpr size_t quadVertexCount = 4;
        const glm::vec2* subTexCoords = subtexture->GetCoords();
        Ref<Texture2D> texture = subtexture->GetTexture();

        float textureIndex = 0.0f;

        glm::mat4 new_transform = transform
            * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));

        QuadGetTextureIndex(texture, textureIndex);
        QuadTransportGLSL(new_transform, tintColor, textureIndex, tilingFactor, subTexCoords);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
    {
        DrawRotatedQuad({ position.x,position.y,0.0f }, size, rotation, color);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
    {
        SL_PROFILE_FUNCTION();

        if (s_Data.QuadIndexCount >= s_Data.MaxIndices) {
            FlushAndReset();
        }

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
            * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f))
            * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        const float textureIndex = 0.0f;
        const float tilingFactor = 1.0f;

        constexpr glm::vec2 texCoords[4] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
        QuadTransportGLSL(transform, color, textureIndex, tilingFactor,texCoords);

    }

    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
    {
        DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
    {
        SL_PROFILE_FUNCTION();

        if (s_Data.QuadIndexCount >= s_Data.MaxIndices) {
            FlushAndReset();
        }

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
            * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f))
            * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        float textureIndex = 0.0f;
        for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
            if (*s_Data.Textures[i].get() == *texture.get()) {
                textureIndex = (float)i;
                break;
            }
        }
        if (textureIndex == 0.0f) {
            // 如果纹理数也多于最大值，就刷新一次批渲染
            if (s_Data.TextureSlotIndex >= s_Data.MaxTextureSlots) {
                FlushAndReset();
            }

            s_Data.Textures[s_Data.TextureSlotIndex] = texture;
            textureIndex = float(s_Data.TextureSlotIndex);

            s_Data.TextureSlotIndex++;
        }

        constexpr glm::vec2 texCoords[4] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
        QuadTransportGLSL(transform, tintColor, textureIndex, tilingFactor,texCoords);

    }

    void Renderer2D::DrawRotatedQuad(
        const glm::vec2& position, const glm::vec2& size, float rotation,
        const Ref<SubTexture2D>& subtexture, float tilingFactor,
        const glm::vec4& tintColor) {
        DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, subtexture, tilingFactor, tintColor);
    }

    void Renderer2D::DrawRotatedQuad(
        const glm::vec3& position, const glm::vec2& size, float rotation,
        const Ref<SubTexture2D>& subtexture, float tilingFactor,
        const glm::vec4& tintColor) {
        SL_PROFILE_FUNCTION();

        if (s_Data.QuadIndexCount >= s_Data.MaxIndices) {
            FlushAndReset();
        }

        constexpr size_t quadVertexCount = 4;
        const glm::vec2* subTexCoords = subtexture->GetCoords();
        Ref<Texture2D>texture = subtexture->GetTexture();

        float textureIndex = 0.0f;

        QuadGetTextureIndex(texture, textureIndex);

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
            * glm::rotate(glm::mat4(1.0f),rotation,glm::vec3(0.0f,0.0f,1.0f))
            * glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });

        QuadTransportGLSL(transform, tintColor, textureIndex, tilingFactor,subTexCoords);
    }

    void Renderer2D::ClearStats()
    {
        //s_Data.Stats.DrawCalls = 0;
        //s_Data.Stats.QuadCount = 0;
        memset(&s_Data.Stats, 0, sizeof(Statistics));
    }

    Renderer2D::Statistics Renderer2D::GetStats()
    {
        return s_Data.Stats;
    }
    void Renderer2D::DrawSprite(const glm::mat4& transform, const SpriteComponent& src, const int& entityID)
    {
        if (src.Texture)
            DrawQuad(transform, src.Texture, src.TilingFactor, src.Color, entityID);
        else
            DrawQuad(transform, src.Color, entityID);
    }
    void Renderer2D::PlayAnimation(const glm::mat4& transform, AnimationComponent& anc, const int& entityID,float ts)
    {
        if (anc.CurrentAnimation) {
            anc.AnimationTimer += ts;
            DrawQuad(transform, anc.CurrentAnimation->getFrame(anc.AnimationTimer).image, 1.0f, { 1.0f, 1.0f, 1.0f, 1.0f }, entityID);
        }
    }
}
