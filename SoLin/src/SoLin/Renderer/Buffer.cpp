#include "slpch.h"
#include"Buffer.h"

#include"Platform/OpenGL/OpenGLBuffer.h"

namespace SoLin {

	//这两个静态函数定义或许放在OpenGLBuffer之中更好，因为与其相关

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size) {
		return new OpenGLVertexBuffer(vertices, size);
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count) {
		return new OpenGLIndexBuffer(indices, count);
	}
}