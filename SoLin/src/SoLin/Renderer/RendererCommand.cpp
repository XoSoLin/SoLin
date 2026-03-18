#include"slpch.h"
#include"RendererCommand.h"

#include"Platform/OpenGL/OpenGLRendererAPI.h"

namespace SoLin {
	RendererAPI* RendererAPI::Create() {
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None: SL_CORE_ASSERT(false, "RendererAPI::None is currently not supported! ");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLRendererAPI();
		case RendererAPI::API::DirectX: SL_CORE_ASSERT(false, "RendererAPI::DirectX is currently not supported! ");
			return nullptr;
		}
		SL_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}

	RendererAPI* RendererCommand::s_RendererAPI = RendererAPI::Create();
}