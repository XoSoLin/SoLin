#include "slpch.h"
#include"RendererAPI.h"

#include"Platform/OpenGL/OpenGLRendererAPI.h"

namespace SoLin {
	// 默认使用OpenGL
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;			//静态变量需要 1.在类外 2.被初始化

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
}
