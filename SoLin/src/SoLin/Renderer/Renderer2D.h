#pragma once
#include"SoLin/Renderer/Camera.h"
#include "OrthoGraphicCamera.h"
#include "SoLin/Renderer/EditorCamera.h"

#include "Texture.h"
#include "SubTexture2D.h"

#include "SoLin/Scene/Component.h"

namespace SoLin {

	//@brief 专用于渲染2D效果的渲染器
	class Renderer2D
	{
	public:
		//@brief 初始化2D渲染
		static void Init();
		//@brief 关闭2D渲染
		static void Shutdown();

        //@brief 开始布景
        //@param camera 存储投影矩阵的相机
        //@param viewMatrix 视口矩阵
        static void BeginScene(const Camera& camera,const glm::mat4& viewMatrix);
		//@brief 开始布景
		static void BeginScene(const OrthoGraphicCamera& camera);
        static void BeginScene(const EditorCamera& camera);
		//@brief 结束布景
		static void EndScene();

        static void Flush();
        static void FlushAndReset();

    private:
        //@brief 将GLSL的必要数据传输给CPU上的缓冲区，后续一起更新至GPU
        static void QuadTransportGLSL(const glm::mat4& transform, const glm::vec4& color, const float& textureIndex, const float& tilingFactor,const glm::vec2* texcoords, const int entityID = -1);
        //@brief 寻找纹理索引
        static void QuadGetTextureIndex(const Ref<Texture2D>& texture,float& index);

    public:
        //--------------------图元---------------------------

        //@brief 画四边形
        //@param transform 变换矩阵
        //@param color 颜色
        static void DrawQuad(const glm::mat4 transform, const glm::vec4 color, const int& entityID = -1);
        //@brief 画纹理四边形
        //@param transform 变换矩阵
        //@param texture 纹理
        //@param tilingFactor 填充因子
        //@param tintColor 色调
        static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), const int& entityID = -1);
        //@brief 画子纹理四边形
        //@param transform 变换矩阵
        //@param subtexture 子纹理
        //@param tilingFactor 填充因子
        //@param tintColor 色调
        static void DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D>& subtexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), const int& entityID = -1);

		//@brief 画四边形
		//@param position 位置
		//@param size 宽高
		//@param color 颜色
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		//@brief 画四边形
		//@param position 位置
		//@param size 宽高
		//@param color 颜色
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		//@brief 画纹理四边形
		//@param position 位置
		//@param size 宽高
		//@param texture 纹理
        //@param tilingFactor 填充因子
        //@param tintColor 色调
        static void DrawQuad(
            const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture,
            float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		//@brief 画纹理四边形
		//@param position 位置
		//@param size 宽高
		//@param texture 纹理
        //@param tilingFactor 填充因子
        //@param tintColor 色调
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture,
            float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

        //@brief 画子纹理四边形
        //@param position 位置
        //@param size 宽高
        //@param subtexture 子纹理
        //@param tilingFactor 填充因子
        //@param tintColor 色调
        static void DrawQuad(
            const glm::vec2& position, const glm::vec2& size,
            const Ref<SubTexture2D>& subtexture, float tilingFactor = 1.0f,
            const glm::vec4& tintColor = glm::vec4(1.0f));
        //@brief 画子纹理四边形
        //@param position 位置
        //@param size 宽高
        //@param subtexture 子纹理
        //@param tilingFactor 填充因子
        //@param tintColor 色调
        static void DrawQuad(
            const glm::vec3& position, const glm::vec2& size,
            const Ref<SubTexture2D>& subtexture, float tilingFactor = 1.0f,
            const glm::vec4& tintColor = glm::vec4(1.0f));


        //@brief 画可旋四边形
        //@param transform 变换矩阵
        //@param rotation 旋转角度
        //@param color 颜色
        static void DrawRotatedQuad(const glm::mat4& transform, float rotation, const glm::vec4& color);
        //@brief 画可旋纹理四边形
        //@param transform 变换矩阵
        //@param rotation 旋转角度
        //@param texture 纹理
        //@param tilingFactor 填充因子
        //@param tintColor 色调
        static void DrawRotatedQuad(const glm::mat4& transform, float rotation, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
        //@brief 画可旋子纹理四边形
        //@param transform 变换矩阵
        //@param rotation 旋转角度
        //@param subtexture 子纹理
        //@param tilingFactor 填充因子
        //@param tintColor 色调
        static void DrawRotatedQuad(const glm::mat4& transform, float rotation, const Ref<SubTexture2D>& subtexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

        //@brief 画可旋四边形
        //@param position 位置
        //@param size 宽高
        //@param rotation 旋转角度
        //@param color 颜色
        static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
        //@brief 画可旋四边形
        //@param position 位置
        //@param size 宽高
        //@param rotation 旋转角度
        //@param color 颜色
        static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
        //@brief 画可旋纹理四边形
        //@param position 位置
        //@param size 宽高
        // @param rotation 旋转角度
        //@param texture 纹理
        //@param tilingFactor 填充因子
        //@param tintColor 色调
        static void DrawRotatedQuad(
            const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture,
            float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
        //@brief 画可旋纹理四边形
        //@param position 位置
        //@param size 宽高
        //@param rotation 旋转角度
        //@param texture 纹理
        //@param tilingFactor 填充因子
        //@param tintColor 色调
        static void DrawRotatedQuad(
            const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture,
            float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

        //@brief 画可旋子纹理四边形
        //@param position 位置
        //@param size 宽高
        //@param rotation 旋转角度
        //@param subtexture 子纹理
        //@param tilingFactor 填充因子
        //@param tintColor 色调
        static void DrawRotatedQuad(
            const glm::vec2& position, const glm::vec2& size, float rotation,
            const Ref<SubTexture2D>& subtexture, float tilingFactor = 1.0f,
            const glm::vec4& tintColor = glm::vec4(1.0f));
        //@brief 画可旋子纹理四边形
        //@param position 位置
        //@param size 宽高
        //@param rotation 旋转角度
        //@param subtexture 子纹理
        //@param tilingFactor 填充因子
        //@param tintColor 色调
        static void DrawRotatedQuad(
            const glm::vec3& position, const glm::vec2& size, float rotation,
            const Ref<SubTexture2D>& subtexture, float tilingFactor = 1.0f,
            const glm::vec4& tintColor = glm::vec4(1.0f));

        //-----------------------------精灵------------------------------------
        static void DrawSprite(const glm::mat4& transform, const SpriteComponent& src, const int& entityID);

        struct Statistics {
            uint32_t DrawCalls;
            uint32_t QuadCount;

            // 函数被调用时再计算Vertex或Index，节省性能
            uint32_t GetVertexCount() { return QuadCount * 4; }
            uint32_t GetIndexCount() { return QuadCount * 6; }
        };

        static void ClearStats();
        static Statistics GetStats();
	};

}
