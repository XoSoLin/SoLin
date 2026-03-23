#pragma once
#include <string>

#include<glm/glm.hpp>

namespace SoLin {
	class Shader {
	public:
		virtual ~Shader() = default;

		virtual void Bind()const = 0;
		virtual void UnBind()const = 0;
		virtual const std::string& GetName() const = 0;

		static Ref<Shader> Create(const std::string& filepath);
		static Ref<Shader> Create(const std::string& name,const std::string& vertexSrc, const std::string& fragmentSrc);
	};

	//@brief 着色器库
	// 本质就是一个name到shader的映射表
	class ShaderLibrary {
	public:
		void Add(const Ref<Shader>& shader);
		void Add(const std::string& name, const Ref<Shader>& shader);

		//@brief 加载着色器到库
		//@param filepath 文件路径
		//@return 返回该加载完成的着色器
		Ref<Shader> Load(const std::string& filepath);
		//@brief 加载着色器库
		//@param name 着色器名
		//@param filepath 文件路径
		//@return 返回该加载完成的着色器
		Ref<Shader> Load(const std::string& name, const std::string& filepath);

		Ref<Shader> Get(const std::string& name);

		//@brief 判断是否存在
		bool Exists(const std::string& name) const;
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;

	};
}