#include "slpch.h"
#include "FrameBuffer.h"

#include"SoLin/Renderer/Renderer.h"
#include"Platform/OpenGL/OpenGLFrameBuffer.h"

namespace SoLin {
    Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec) {
        switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::None:
            SL_CORE_ASSERT(false, "RendererAPI::None is currently supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return CreateRef<OpenGLFrameBuffer>(spec);
        }
        SL_CORE_ASSERT(false, "Unknown RendererAPI");
        return nullptr;
    }
}
