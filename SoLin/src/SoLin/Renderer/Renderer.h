#pragma once

namespace SoLin {

	//@brief äÖÈ¾APIÃ¶¾Ù
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

}