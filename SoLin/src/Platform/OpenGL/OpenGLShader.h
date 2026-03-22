#pragma once

#include "SoLin/Renderer/Shader.h"

typedef unsigned int GLenum;

namespace SoLin {
	class OpenGLShader :public Shader {
	public:

		//@param 着色器源码文件路径
		OpenGLShader(const std::string& filepath);

		//@param vertexSrc 顶点着色器源码
		//@param fragmentSrc 片段着色器源码
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		void UpdateUniformInt(const std::string& name, const int& value);

		void UpdateUniformFloat(const std::string& name, const float& value);
		void UpdateUniformFloat2(const std::string& name, const glm::vec2& value);
		void UpdateUniformFloat3(const std::string& name, const glm::vec3& value);
		void UpdateUniformFloat4(const std::string& name, const glm::vec4& value);

		void UpdateUniformMat3(const std::string& name, const glm::mat3& matrix);
		//@brief 更新四维矩阵统一变量
		//@param name 统一变量名
		//@param matrix 四维矩阵指针
		void UpdateUniformMat4(const std::string& name, const glm::mat4& matrix);
	private:
		//@brief 读取文件
		std::string ReadFile(const std::string& filepath);

		//@brief 将各种着色器组成的源码分类
		//@param source 混合的源码
		//@return 着色器源码映射表
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);

		//@brief 编译着色器程序
		//@param shaderSource 着色器源码映射表
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSource);
	private:
		uint32_t m_RendererID;
	};
}