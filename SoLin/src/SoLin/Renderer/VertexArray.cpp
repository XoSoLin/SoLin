#include"slpch.h"
#include"VertexArray.h"

#include"SoLin/Renderer/RendererAPI.h"
#include"Platform/OpenGL/OpenGLVertexArray.h"

namespace SoLin {
	
	VertexArray* VertexArray::Create() {
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::None:
			SL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLVertexArray();
		}

		SL_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
}