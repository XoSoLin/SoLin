#pragma once

namespace SoLin {

	// 渲染上下文接口
	class GraphicsContext {				//将其抽象，增加其普适性，使得后续OpenGL、Vulkan、DirectX均可适用
	public:
        virtual ~GraphicsContext() = default;

		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

        static std::unique_ptr<GraphicsContext> Create(void* window);
	private:

	 };

}
