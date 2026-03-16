#pragma once

namespace SoLin {

	//@brief 渲染API枚举
	enum class RendererAPI {
		None,
		OpenGL,
		DirectX
	};

	class Renderer {
	public:
		static inline RendererAPI SetAPI(RendererAPI api) { s_API = api; }
		static inline RendererAPI GetAPI() { return s_API; }
	private:
		static RendererAPI s_API;
	};

	// 默认使用OpenGL
	RendererAPI Renderer::s_API = RendererAPI::OpenGL;			//静态变量需要 1.在类外 2.被初始化
}