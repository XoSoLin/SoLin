#pragma once
#include<glm/glm.hpp>

#include"VertexArray.h"
namespace SoLin {

	class RendererAPI {
	public:
		//@brief 渲染API枚举
		enum class API {
			None,
			OpenGL,
			DirectX
		};
	public:
		// 于各渲染API去定义
		static Scope<RendererAPI> Create();

		virtual void Clear() = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;

		virtual void Init() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;

		static inline API SetAPI(API api) { s_API = api; }
		static inline API GetAPI() { return s_API; }
	private:
		static API s_API;
	};

}