#pragma once

#include"RendererCommand.h"

#include"OrthoGraphicCamera.h"
#include"Shader.h"

namespace SoLin {
	
	//@brief 渲染器
    //@brief 利用 着色器 与 顶点数组 等GPU数据以 指令类 间接进行操作
	class Renderer {
	public:
		static void Init();
		//@brief 窗口调整大小
		//@param width 宽度像素
		//@param height 高度像素
		static void OnWindowResize(uint32_t width, uint32_t height);

		//@brief 渲染器开始渲染场景
		//@param camera 传入相机使用其中必要的数据
		static void BeginScene(OrthoGraphicCamera& camera);
		static void EndScene();

		//@brief 用于渲染对应的着色器
		//@param shader 传入的具体着色器程序
		//@param vertexArray 传入顶点数组供着色器使用
		//@param transform 变化矩阵
		static void Submit(
			const Ref<Shader>& shader,
			const Ref<VertexArray>& vertexArray,
			const glm::mat4& transform
		);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		//@brief 场景数据结构
		struct SceneData {
			glm::mat4 ViewProjectionMatrix;
		};
		static Scope<SceneData> s_SceneData;
	};
}
