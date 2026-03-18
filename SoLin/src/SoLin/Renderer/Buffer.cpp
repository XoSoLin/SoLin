#include "slpch.h"
#include"Buffer.h"

#include"RendererAPI.h"
#include"Platform/OpenGL/OpenGLBuffer.h"

namespace SoLin {

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size) {
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::None:
			SL_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLVertexBuffer(vertices, size);
		case RendererAPI::API::DirectX:
			SL_CORE_ASSERT(false, "RendererAPI::API::DirectX is currently not supported!");
			return nullptr;
		}
		SL_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count) {

		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::None:
			SL_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLIndexBuffer(indices, count);
		case RendererAPI::API::DirectX:
			SL_CORE_ASSERT(false, "RendererAPI::API::DirectX is currently not supported!");
			return nullptr;
		}
		SL_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
}