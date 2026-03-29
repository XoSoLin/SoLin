#include"slpch.h"
#include"OpenGLRendererAPI.h"

#include<glad/glad.h>

namespace SoLin {
	void OpenGLRendererAPI::Init() {
		//启用混合
		glEnable(GL_BLEND);
		// 设置混合函数：定义源因子和目标因子
		// GL_SRC_ALPHA：源因子为源颜色的 Alpha 值（新绘制的像素透明度）
		// GL_ONE_MINUS_SRC_ALPHA：目标因子为 1 减去源颜色的 Alpha 值（背景像素的不透明度）
		// 最终颜色 = 源颜色 * 源Alpha + 目标颜色 * (1 - 源Alpha)
		// 这是实现标准透明混合（Alpha Blending）的常用组合
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//启用深度测试
		glEnable(GL_DEPTH_TEST);
	}
	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		// 视口原点位置x,y
		// 视口的宽高 w,h
		glViewport(x, y, width, height);
	}
	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}
	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
		// 渲染完后解绑当前纹理，避免影响后续纹理渲染
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}