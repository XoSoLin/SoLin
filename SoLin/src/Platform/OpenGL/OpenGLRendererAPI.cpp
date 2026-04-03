#include"slpch.h"
#include"OpenGLRendererAPI.h"

#include<glad/glad.h>

namespace SoLin {

    #pragma region 报错信息回调函数(OpenGLMessageCallback)定义
        void OpenGLMessageCallback
        (unsigned source, unsigned type, unsigned id, unsigned severity, int length, const char* message, const void* userParam)
        {
            switch (severity)
            {
            case GL_DEBUG_SEVERITY_HIGH:         SL_CORE_CRITICAL(message); return;
            case GL_DEBUG_SEVERITY_MEDIUM:       SL_CORE_ERROR(message); return;
            case GL_DEBUG_SEVERITY_LOW:          SL_CORE_WARN(message); return;
            case GL_DEBUG_SEVERITY_NOTIFICATION: SL_CORE_TRACE(message); return;
            }

            SL_CORE_ASSERT(false, "Unknown severity level!");
        }
    # pragma endregion

	void OpenGLRendererAPI::Init() {
        SL_PROFILE_FUNCTION();

        #ifdef SL_DEBUG
            //控制OpenGL生成的调试消息的生成和过滤
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

            glDebugMessageCallback(OpenGLMessageCallback, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
        #endif

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
