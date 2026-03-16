#pragma once
#include "SoLin/Renderer/Buffer.h"

namespace SoLin {

	//////////////////////////////////////////////////////
	/////////////////// VertexBuffer /////////////////////
	//////////////////////////////////////////////////////

	//@brief OpenGL顶点缓冲区
	class OpenGLVertexBuffer :public VertexBuffer {
	public:
		//@param vertices 顶点数组指针
		//@param size 顶点缓冲区Byte大小
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		~OpenGLVertexBuffer();

		virtual void Bind()const override;
		virtual void UnBind()const override;

		virtual const BufferLayout& GetLayout()const override { return m_Layout; }
		virtual void SetLayout(const BufferLayout& layout)override { m_Layout = layout; }
	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
	};

	//////////////////////////////////////////////////////
	/////////////////// IndexBuffer //////////////////////
	//////////////////////////////////////////////////////

	//@brief OpenGL索引缓冲区
	class OpenGLIndexBuffer :public IndexBuffer {
	public:
		// @param indices 索引数组指针
		// @param count 索引总数量
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void UnBind() const override;
		virtual uint32_t GetCount() const override { return m_Count; }
	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};
}