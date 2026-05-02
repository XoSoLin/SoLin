#include"slpch.h"
#include"SoLin/Renderer/UniformBuffer.h"

#include"SoLin/Renderer/RendererAPI.h"
#include"Platform/OpenGL/OpenGLUniformBuffer.h"

namespace SoLin {
    Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding) {
        switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::None:
            SL_CORE_ASSERT(false, "RendererAPI::None is currently not supported! ")
            return nullptr;
        case RendererAPI::API::OpenGL:
            return CreateRef<OpenGLUniformBuffer>(size, binding);
        }

        SL_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}
