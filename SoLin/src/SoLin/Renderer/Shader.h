#pragma once
#include <string>

#include<glm/glm.hpp>

namespace SoLin {
	class Shader {
	public:
		//@param vertexSrc 顶点着色器源码
		//@param fragmentSrc 片段着色器源码
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void Bind()const;
		void UnBind()const;

		//@brief 更新四维矩阵统一变量
		//@param name 统一变量名
		//@param matrix 四维矩阵指针
		void UpdateUniformMat4(const std::string& name, const glm::mat4& matrix);
	private:
		uint32_t m_RendererID;
	};
}