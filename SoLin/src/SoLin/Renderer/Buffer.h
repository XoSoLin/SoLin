#pragma once

namespace SoLin {

	//////////////////////////////////////////////////////
	/////////////////// VertexBuffer /////////////////////
	//////////////////////////////////////////////////////

	//@brief 顶点缓冲区接口
	class VertexBuffer
	{
	public:
		virtual  ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		//@param vertices 顶点数组指针
		//@param size 顶点缓冲区Byte大小
		static VertexBuffer* Create(float* vertices, uint32_t size);
	};

	//////////////////////////////////////////////////////
	/////////////////// IndexBuffer //////////////////////
	//////////////////////////////////////////////////////

	//@brief 索引缓冲区接口
	class IndexBuffer
	{
	public:
		virtual  ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
		virtual uint32_t GetCount() const = 0;

		// @param indices 索引数组指针
		// @param count 索引总数量
		static IndexBuffer* Create(uint32_t* indices, uint32_t count);
	};
}