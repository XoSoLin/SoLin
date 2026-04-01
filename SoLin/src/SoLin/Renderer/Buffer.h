#pragma once

namespace SoLin {

	//////////////////////////////////////////////////////
	/////////////////// BufferLayout /////////////////////
	//////////////////////////////////////////////////////
	
	//@brief 着色器数据布局类型枚举
	enum class ShaderDataType :uint8_t {
		None,Float,Float2,Float3,Float4,Int,Int2,Int3,Int4,Mat3,Mat4,Bool
	};

	//@param type 着色器数据布局类型
	//@return 着色器数据布局类型大小(byte)
	static uint32_t ShaderDataTypeSize(ShaderDataType type) {
		switch (type) {
		case ShaderDataType::Float:		return 4;
		case ShaderDataType::Float2:	return 4 * 2;
		case ShaderDataType::Float3:	return 4 * 3;
		case ShaderDataType::Float4:	return 4 * 4;
		case ShaderDataType::Int:		return 4;
		case ShaderDataType::Int2:		return 4 * 2;
		case ShaderDataType::Int3:		return 4 * 3;
		case ShaderDataType::Int4:		return 4 * 4;
		case ShaderDataType::Mat3:		return 4 * 3 * 3;
		case ShaderDataType::Mat4:		return 4 * 4 * 4;
		case ShaderDataType::Bool:		return 1;
		}
		SL_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}


	// @brief 布局元素数据结构
	struct LayoutElement {
		std::string Name;		/// 布局元素名
		ShaderDataType Type;	/// 着色器数据布局类型
		uint32_t Size;			/// 布局元素大小(byte)
		uint32_t Offset;		/// 偏移量
		uint32_t Count;			/// 布局元素中的变量个数
		bool Normalized;		/// 是否标准化
		uint32_t GLType;		/// 基础变量的GL类型

		// @brief 
		LayoutElement(){}

		// @param type 着色器数据布局类型
		// @param name 该布局元素名
		// @param normalized 是否标准化，默认为f
		LayoutElement(ShaderDataType type,const std::string& name,bool normalized = false)
			:Type(type),Name(name),Size(ShaderDataTypeSize(type)),Offset(0),Count(GetComponentCount()),Normalized(normalized),GLType(GetTypeToGLType(type))
		{}

		// @brief 通过存储的ShaderDataType计算组成该类型的基本元素个数
		// @return 组成个数
		uint32_t GetComponentCount() const {
			switch (Type) {
			case ShaderDataType::Float:		return 1;
			case ShaderDataType::Float2:	return 2;
			case ShaderDataType::Float3:	return 3;
			case ShaderDataType::Float4:	return 4;
			case ShaderDataType::Int:		return 1;
			case ShaderDataType::Int2:		return 2;
			case ShaderDataType::Int3:		return 3;
			case ShaderDataType::Int4:		return 4;
			case ShaderDataType::Mat3:		return 3 * 3;
			case ShaderDataType::Mat4:		return 4 * 4;
			case ShaderDataType::Bool:		return 1;
			}
			SL_CORE_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}

		// @brief 将着色器数据类型转换为其对应的GL类型
		uint32_t GetTypeToGLType(ShaderDataType type) {
			switch (type) {
			case ShaderDataType::Float:		return 0x1406;			//GL_FLOAT == 0x1406
			case ShaderDataType::Float2:	return 0x1406;
			case ShaderDataType::Float3:	return 0x1406;
			case ShaderDataType::Float4:	return 0x1406;
			case ShaderDataType::Int:		return 0x1404;			//GL_INT == 0x1404
			case ShaderDataType::Int2:		return 0x1404;
			case ShaderDataType::Int3:		return 0x1404;
			case ShaderDataType::Int4:		return 0x1404;
			case ShaderDataType::Mat3:		return 0x1406;			//GL_FLOAT == 0x1406
			case ShaderDataType::Mat4:		return 0x1406;
			case ShaderDataType::Bool:		return 0x8B56;			//GL_BOOL == 0x8B56
			}
			SL_CORE_ASSERT(false, "Unknown ShaderDataType !");
			return 0;
		}

	};

	// @brief 缓冲区布局
	class BufferLayout {
	public:
		BufferLayout(){}
		BufferLayout(std::initializer_list<LayoutElement> elements)
			:m_Elements(elements) {
			CalcOffsetAndStride();
		}
		inline const std::vector<LayoutElement>& GetElements()const { return m_Elements; }
		inline const uint32_t& GetStride()const { return m_Stride; }

		std::vector<LayoutElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<LayoutElement>::iterator end() { return m_Elements.end(); }
		std::vector<LayoutElement>::const_iterator begin()const { return m_Elements.begin(); }
		std::vector<LayoutElement>::const_iterator end()const { return m_Elements.end(); }

	private:
		std::vector<LayoutElement> m_Elements;		/// 布局元素列表
		uint32_t m_Stride;							/// 整个布局步长(byte)
	private:
		//@brief 计算每个元素的偏移和整体布局的步长，在初始化时使用
		void CalcOffsetAndStride() {
			m_Stride = 0;
			uint32_t offset = 0;
			for (auto& element : m_Elements) {
				element.Offset = offset;		//为元素设置偏移，第一个的为 0，后续累加
				offset += element.Size;
				m_Stride += element.Size;		//Size 在 LayoutElement 的构造函数中被计算了
			}
		}
	};

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

		virtual const BufferLayout& GetLayout()const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

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