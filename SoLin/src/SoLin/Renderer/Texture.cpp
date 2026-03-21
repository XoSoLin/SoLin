#include "slpch.h"
#include"Texture.h"

#include "Renderer.h"
#include"Platform/OpenGL/OpenGLTexture2D.h"

namespace SoLin {

	Ref<Texture2D> Texture2D::Create(const std::string& path) {

		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			SL_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLTexture2D>(path);
		}

		SL_CORE_ASSERT(false, "Unknown Renderer API!");
	}
}