#include<slpch.h>
#include"Shader.h"

#include"SoLin/Renderer/Renderer.h"
#include"Platform/OpenGL/OpenGLShader.h"

namespace SoLin {
	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			SL_CORE_ASSERT(false, "RendererAPI::API::None is currently not suppotted!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLShader(vertexSrc, fragmentSrc);

		}
		SL_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}

}