#pragma once

#include "SoLin/Renderer/Shader.h"

namespace SoLin {
	class OpenGLShader :public Shader {
	public:

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
		uint32_t m_RendererID;
	};
}