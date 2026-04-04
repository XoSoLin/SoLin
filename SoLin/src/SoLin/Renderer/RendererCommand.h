#pragma once
#include"RendererAPI.h"

namespace SoLin {
	
	//@brief 渲染命令
	//@brief 根据渲染API的不同，会调用不同的渲染指令
	class RendererCommand {
	public:
		inline static void Init() {
			s_RendererAPI->Init();
		}
		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
			s_RendererAPI->SetViewport(x, y, width, height);
		}
		inline static void Clear() {
			s_RendererAPI->Clear();
		}
		inline static void SetClearColor(const glm::vec4& color) {
			s_RendererAPI->SetClearColor(color);
		}
		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray) {
			s_RendererAPI->DrawIndexed(vertexArray);
		}
        inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount) {
            s_RendererAPI->DrawIndexed(vertexArray, indexCount);
        }
	private:
		static Scope<RendererAPI> s_RendererAPI;
	};
}
