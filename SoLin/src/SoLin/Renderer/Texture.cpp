#include "slpch.h"
#include"Texture.h"

#include "Renderer.h"
#include"Platform/OpenGL/OpenGLTexture2D.h"

namespace SoLin {

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			SL_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(width, height);
		}

		SL_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::filesystem::path& path) {

        // 打开文件失败就返回空
        FILE* file = SLOPEN(path);
        if (!file) {
            return nullptr;
        }
        fclose(file);

		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			SL_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(path);
		}

		SL_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
}
