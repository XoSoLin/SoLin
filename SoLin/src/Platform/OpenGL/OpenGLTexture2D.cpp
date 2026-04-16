#include "slpch.h"
#include"OpenGLTexture2D.h"

#include<glad/glad.h>

#include<stb_image.h>

namespace SoLin {
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		:m_Width(width),m_Height(height)
	{
        SL_PROFILE_FUNCTION();

		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);			// 储存数据(width 和 height 表示像素点个数形成的宽与高）

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);				//纹理过滤
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);					//纹理环绕模式
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::filesystem::path& path)
		:m_Path(path)
	{
        SL_PROFILE_FUNCTION();

		// 启用stb_image 垂直翻转，使图像原点与 OpenGL 纹理坐标一致
		// （OpenGL 原点在左下角，图像原点在左上角）
		stbi_set_flip_vertically_on_load(1);

		int width, height, channels;
		// 使用 stb_image 加载图像文件
		// &width, &height, &channels：用于接收图像的宽度、高度和通道数（例如 3=RGB, 4=RGBA）
		// 最后一个参数 0 表示保持图像的原始通道数
        FILE* file = SLOPEN(path);
        SL_CORE_ASSERT(file, "Failed to open file!");
		//stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        stbi_uc* data = stbi_load_from_file(file, &width, &height, &channels, 0);
		SL_CORE_ASSERT(data, "Failed to load image!");
        fclose(file);

		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0;			//数据内部存储格式 
		GLenum dataFormat = 0;				//数据的上传格式
		if (channels == 3) {
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}
		else if (channels == 4) {
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		SL_CORE_ASSERT((internalFormat & dataFormat), "Format not supported!");

		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;

		// GL_TEXTURE_2D：纹理类型为 2D 纹理
		// 1：创建 1 个纹理对象
		// &m_RendererID：返回的纹理对象 ID 存储在此变量中
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);

		// 为纹理分配不可变的存储空间（尺寸和格式固定）
		// m_RendererID：纹理对象 ID
		// 1：只分配基础 mipmap 级别（不生成多级纹理）
		// GL_RGBA8：内部格式，每个像素 8 位 RGBA（共 32 位）
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat,m_Width,m_Height);

		// 设置纹理缩小过滤器：当纹理被缩小时，使用线性插值采样（平滑过渡）
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// 设置纹理放大过滤器：当纹理被放大时，使用最近邻采样（保持像素边缘清晰）
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		//设置纹理环绕方式，S为水平，T为垂直
		//GL_REPEAT 表示重复纹理
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// 将图像数据上传到纹理的存储空间中
		// m_RendererID：纹理对象 ID
		// 0：上传到 mipmap 级别 0（基础级别）
		// 0, 0：从纹理的 (0,0) 坐标开始填充
		// m_Width, m_Height：要填充的宽度和高度（覆盖整个纹理）
		// GL_RGBA：源数据的像素格式（RGBA 四通道）
		// GL_UNSIGNED_BYTE：源数据的每个通道类型（无符号字节）
		// data：图像数据的指针
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);
		
		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
        SL_PROFILE_FUNCTION();

		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
        SL_PROFILE_FUNCTION();

		uint32_t bpp = (m_DataFormat == GL_RGBA ? 4 : 3);
		SL_CORE_ASSERT((size == m_Width * m_Height * bpp), 
			"Data must contain the full texture! Please check that the size of the data matches the format of the data"
		);
		// 上传数据
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat,GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
        SL_PROFILE_FUNCTION();

		glBindTextureUnit(slot, m_RendererID);
	}
}
