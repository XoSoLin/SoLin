#include"slpch.h"
#include"GraphicsContext.h"

#include"SoLin/Renderer/Renderer.h"
#include"Platform/OpenGL/OpenGLContext.h"

namespace SoLin {
    std::unique_ptr<GraphicsContext> GraphicsContext::Create(void* window) {
        switch (Renderer::GetAPI()) {
        case RendererAPI::API::None:
            SL_CORE_ASSERT(false, "None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return std::make_unique<OpenGLContext>(static_cast<GLFWwindow*>(window));
        }

        SL_CORE_ASSERT(false, "Unkown RendererAPI!");
        return nullptr;
    }

}
