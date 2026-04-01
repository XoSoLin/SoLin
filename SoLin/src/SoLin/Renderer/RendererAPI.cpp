#include "slpch.h"
#include"RendererAPI.h"

namespace SoLin {
	// 默认使用OpenGL
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;			//静态变量需要 1.在类外 2.被初始化
}