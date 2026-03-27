#include"slpch.h"
#include"VertexArray.h"

#include"SoLin/Renderer/RendererAPI.h"
#include"Platform/OpenGL/OpenGLVertexArray.h"

namespace SoLin {
	
	Ref<VertexArray> VertexArray::Create() {
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::None:
			SL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLVertexArray>();
		}

		SL_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
}