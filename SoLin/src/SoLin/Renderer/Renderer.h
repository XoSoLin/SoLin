#pragma once

#include"RendererCommand.h"

#include"OrthoGraphicCamera.h"
#include"Shader.h"

namespace SoLin {
	
	class Renderer {
	public:
		//@brief 渲染器开始渲染场景
		//@param camera 传入相机使用其中必要的数据
		static void BeginScene(OrthoGraphicCamera& camera);
		static void EndScene();

		//@brief 用于渲染对应的着色器
		//@param shader 传入的具体着色器程序
		//@param vertexArray 传入顶点数组供着色器使用
		//@param transform 变化矩阵
		static void Submit(
			const std::shared_ptr<Shader>& shader,
			const std::shared_ptr<VertexArray>& vertexArray,
			const glm::mat4& transform
		);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		//@brief 场景数据结构
		struct SceneData {
			glm::mat4 ViewProjectionMatrix;
		};
		static SceneData* s_SceneData;
	};
}