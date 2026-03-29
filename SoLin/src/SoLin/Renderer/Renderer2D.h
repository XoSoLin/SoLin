#pragma once
#include "OrthoGraphicCamera.h"

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
	};
}