#pragma once
#include"SoLin/Renderer/Texture.h"

#include<glad/glad.h>

namespace SoLin {
	class OpenGLTexture2D :public Texture2D {
	public:
		//@brief 这个构造函数需要使用 SetData()
		//@param width 纹理宽
		//@param height 纹理高
		OpenGLTexture2D(uint32_t width, uint32_t height);
		//@brief 使用图片构造2D纹理
		//@param path 原图片资源路径
		OpenGLTexture2D(const std::filesystem::path& path);
		~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }

		virtual void SetData(void* data, uint32_t size)override;

		virtual void Bind(uint32_t slot = 0)const override;

        virtual bool operator==(const Texture& other)const override {
            return(this->m_RendererID == ((OpenGLTexture2D&)other).m_RendererID);
        }

	private:
		std::filesystem::path m_Path;

		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_RendererID;
		GLenum m_InternalFormat, m_DataFormat;
	};
}
