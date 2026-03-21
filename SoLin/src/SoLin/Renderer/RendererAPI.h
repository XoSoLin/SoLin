#pragma once
#include<glm/glm.hpp>

#include"VertexArray.h"
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
		static RendererAPI* Create();

		virtual void Clear() = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;

		static inline API SetAPI(API api) { s_API = api; }
		static inline API GetAPI() { return s_API; }
	private:
		static API s_API;
	};

}