#include"slpch.h"
#include"RendererCommand.h"

#include"Platform/OpenGL/OpenGLRendererAPI.h"

namespace SoLin {
	Scope<RendererAPI> RendererAPI::Create() {
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None: SL_CORE_ASSERT(false, "RendererAPI::None is currently not supported! ");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateScope<OpenGLRendererAPI>();
		case RendererAPI::API::DirectX: SL_CORE_ASSERT(false, "RendererAPI::DirectX is currently not supported! ");
			return nullptr;
		}
		SL_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}

	Scope<RendererAPI> RendererCommand::s_RendererAPI = RendererAPI::Create();
}