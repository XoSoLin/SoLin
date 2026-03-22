#include"slpch.h"
#include"OpenGLShader.h"

#include<glad/glad.h>
#include<glm/gtc/type_ptr.hpp>

#include <fstream>

namespace SoLin {

	//@brief 从字符串中获取着色器类型
	GLenum ShaderTypeFromString(const std::string& type) {
		if (type == "vertex") {
			return GL_VERTEX_SHADER;
		}
		else if (type == "fragment" || type == "pixel") {
			return GL_FRAGMENT_SHADER;
		}
		SL_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		std::string source = ReadFile(filepath);
		std::unordered_map<GLenum, std::string> shaderSources = PreProcess(source); 
		Compile(shaderSources);
	}

	OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		std::unordered_map<GLenum, std::string> shaderSources;
		shaderSources[GL_VERTEX_SHADER] = vertexSrc;
		shaderSources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(shaderSources);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::UnBind() const
	{
		glUseProgram(0);
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath) {
		std::string result;
		std::ifstream readIn(filepath, std::ios::in | std::ios::binary);		//in|binary 表示以输入模式和二进制模式 打开
		if (readIn) {
			readIn.seekg(0, std::ios::end);							//seekg()移动指针到文件末尾，偏移为0
			result.resize(readIn.tellg());								//tellg()返回当前读取指针的位置，也就是将文件的总字节数

			readIn.seekg(0, std::ios::beg);							//移动指针到文件开头
			readIn.read(&result[0], result.size());					//读取文件内容
			readIn.close();														//关闭文件
		}
		else {
			SL_CORE_ERROR("Could not open file form:'{0}'", filepath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source) {
		std::unordered_map<GLenum, std::string> shaderSources;
		const char* token = "#type";
		size_t tokenLength = strlen(token);
		size_t pos = source.find(token, 0);													//从0开始寻找标记位置

		while (pos != std::string::npos) {																//找到标记就继续
			size_t begin = pos + tokenLength + 1;														
			size_t end_of_line = source.find_first_of("\r\n", pos);							//从位置 pos 开始查找第一个换行符
			SL_CORE_ASSERT((end_of_line != std::string::npos), "Syntax error");
			std::string type = source.substr(begin, end_of_line - begin);
			SL_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", end_of_line);				//从 end_of_line 之后开始查找第一个不是换行符的字符
			pos = source.find(token, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] =
				source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));

		}
		return shaderSources;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources) {

		m_RendererID = glCreateProgram();								//创建着色器程序
		
		//存储产生的着色器
		std::vector<GLuint> glShaders(shaderSources.size());
		for (auto& key_value : shaderSources) {

			GLenum type = key_value.first;								//获取着色器类型
			const std::string& source = key_value.second;				//获取着色器源码
			GLuint shader = glCreateShader(type);						//根据类型创建着色器

			const GLchar* sourceC_str = source.c_str();					//获取c系字符串的源码
			glShaderSource(shader, 1, &sourceC_str, 0);					//为创建的着色器绑定源码

			glCompileShader(shader);									//编译该着色器

			GLint isCompiled = 0;										
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);		//获取是否成功编译信息

			if (isCompiled == GL_FALSE) {
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);	//获取日志长度
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);		//获取日志信息
				glDeleteShader(shader);									//删除着色器
				SL_CORE_ERROR("{0}", infoLog.data());					//输出日志信息
				SL_ASSERT(false, "Shader compilation failure!");
				break;
			}

			glAttachShader(m_RendererID, shader);						//为着色器程序添加这个着色器
			glShaders.push_back(shader);							//存储这个着色器
		}

		glLinkProgram(m_RendererID);									//为程序进行连接

		GLint isLinked = 0;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, (int*)&isLinked);	//获取是否连接成功信息
		if (isLinked == GL_FALSE) {
			GLint maxLength = 0;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);	// 获取日志长度信息

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);	//获取日志信息
			glDeleteProgram(m_RendererID);								//删除该着色器程序
			for (auto shader : glShaders)							//删除所有的着色器
				glDeleteShader(shader);

			SL_CORE_ERROR("{0}", infoLog.data());
			SL_ASSERT(false, "Shader link failure!");
			return;
		}

		for (auto shader : glShaders)								//连接成功后 为着色器程序移除所有的着色器
			glDetachShader(m_RendererID,shader);
	}

	void OpenGLShader::UpdateUniformInt(const std::string& name, const int& value)
	{
		uint32_t location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UpdateUniformFloat(const std::string& name, const float& value)
	{
		uint32_t location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}
	void OpenGLShader::UpdateUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		uint32_t location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, value.x,value.y);
	}
	void OpenGLShader::UpdateUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		uint32_t location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, value.x,value.y,value.z);
	}
	void OpenGLShader::UpdateUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		uint32_t location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, value.x,value.y,value.z,value.w);
	}
	void OpenGLShader::UpdateUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		uint32_t location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
	void OpenGLShader::UpdateUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		uint32_t location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
}