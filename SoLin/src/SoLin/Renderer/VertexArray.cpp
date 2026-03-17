#include"slpch.h"
#include"VertexArray.h"

#include"SoLin/Renderer/Renderer.h"
#include"Platform/OpenGL/OpenGLVertexArray.h"

namespace SoLin {
	
	VertexArray* VertexArray::Create() {
		switch (Renderer::GetAPI()) {
		case RendererAPI::None:
			SL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::OpenGL:
			return new OpenGLVertexArray();
		}

		SL_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
}