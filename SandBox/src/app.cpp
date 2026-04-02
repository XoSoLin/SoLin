#include<SoLin.h>
#include<SoLin/Core/EntryPoint.h>
#include"ExampleLayer.h"
#include"Sandbox2D.h"


class SandBox :public SoLin::Application { 
public:
	SandBox() {
		//取消 PushOverlay(new SoLin::ImGuiLayer()); ，将其作为 SoLin 运行时 固定自动添加的图层
		//PushLayer(new Sandbox2D());
		PushLayer(new ExampleLayer());
	}
	~SandBox() {

	}
};


SoLin::Application* SoLin::CreateApplication() {
	return new SandBox();
}
