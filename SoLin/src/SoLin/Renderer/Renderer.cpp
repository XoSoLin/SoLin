#include "slpch.h"
#include"Renderer.h"

namespace SoLin {
	// 默认使用OpenGL
	RendererAPI Renderer::s_API = RendererAPI::OpenGL;			//静态变量需要 1.在类外 2.被初始化
}