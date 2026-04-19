#include"slpch.h"
#include"OpenGLVertexArray.h"

#include"glad/glad.h"

namespace SoLin {


	OpenGLVertexArray::OpenGLVertexArray()
	{
        SL_PROFILE_FUNCTION();

		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
        SL_PROFILE_FUNCTION();

		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
        SL_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
	}
	void OpenGLVertexArray::UnBind() const
	{
        SL_PROFILE_FUNCTION();

		glBindVertexArray(0);
	}
	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexbuffer)
	{
        SL_PROFILE_FUNCTION();

		SL_CORE_ASSERT(vertexbuffer->GetLayout().GetElements().size(), "顶点缓冲布局现在是空的!");

		glBindVertexArray(m_RendererID);
		vertexbuffer->Bind();
		uint32_t index = 0;
		const auto& layout = vertexbuffer->GetLayout();
		for (const auto& element : layout) {
            switch (element.Type) {
                case ShaderDataType::Int:
                case ShaderDataType::Int2:
                case ShaderDataType::Int3:
                case ShaderDataType::Int4:
                case ShaderDataType::Bool:
                {
                    glEnableVertexAttribArray(index);
                    // 这里不需要插值,EntityID这样的值要整个区域都是同一个值
                    glVertexAttribIPointer(index, element.Count, element.GLType,
                        layout.GetStride(), (const void*)element.Offset
                    );
                    index++;
                    break;
                }
                case ShaderDataType::Float:
                case ShaderDataType::Float2:
                case ShaderDataType::Float3:
                case ShaderDataType::Float4:
                case ShaderDataType::Mat3:
                case ShaderDataType::Mat4:
                {
                    glEnableVertexAttribArray(index);
                    glVertexAttribPointer(index, element.Count, element.GLType,
                        element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)element.Offset
                    );
                    index++;
                    break;
                }
                default:
                    SL_CORE_ASSERT(false, "Unknown ShaderDataType!");
            }
		}

		m_VertexBuffers.push_back(vertexbuffer);
	}
	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexbuffer)
	{
        SL_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
		indexbuffer->Bind();

		m_IndexBuffer = indexbuffer;
	}
}
