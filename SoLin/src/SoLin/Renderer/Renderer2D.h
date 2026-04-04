#pragma once
#include "OrthoGraphicCamera.h"

#include "Texture.h"

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
		static void BeginScene(const OrthoGraphicCamera& camera);
		//@brief 结束布景
		static void EndScene();

        static void Flush();

		//--------------------图元---------------------------

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

        //@brief 画四边形
        //@param position 位置
        //@param size 宽高
        //@param rotation 旋转角度
        //@param color 颜色
        static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
        //@brief 画四边形
        //@param position 位置
        //@param size 宽高
        //@param rotation 旋转角度
        //@param color 颜色
        static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
        //@brief 画纹理四边形
        //@param position 位置
        //@param size 宽高
        // @param rotation 旋转角度
        //@param texture 纹理
        //@param tilingFactor 填充因子
        //@param tintColor 色调
        static void DrawRotatedQuad(
            const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture,
            float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
        //@brief 画纹理四边形
        //@param position 位置
        //@param size 宽高
        // @param rotation 旋转角度
        //@param texture 纹理
        //@param tilingFactor 填充因子
        //@param tintColor 色调
        static void DrawRotatedQuad(
            const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture,
            float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
	};
}
