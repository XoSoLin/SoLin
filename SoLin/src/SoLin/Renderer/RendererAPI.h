#pragma once

namespace SoLin {

	class RendererAPI {
	public:
		//@brief äÖÈ¾APIÃ¶¾Ù
		enum class API {
			None,
			OpenGL,
			DirectX
		};
	public:
		static inline API SetAPI(API api) { s_API = api; }
		static inline API GetAPI() { return s_API; }
	private:
		static API s_API;
	};

}