#include<SoLin.h>

#include "imgui/imgui.h"

class ExampleLayer :public SoLin::Layer {
public:
	ExampleLayer()
		:Layer("Example layer") 
	{
	}

	void OnUpdate() override {
		//SL_INFO("ExampleLayer::OnUpdate");
	} 
	void OnImGuiRender()
	{
		ImGui::Begin("Test"); 
        const char* text = R"(vva)";
        ImGui::Text(text);
		ImGui::End();
	}

	void OnEvent(SoLin::Event& event)override {
		//SL_TRACE("{0}", event.ToString());
		if (event.GetEventType() == SoLin::EventType::KeyPressed) {
			SoLin::KeyPressedEvent& e = (SoLin::KeyPressedEvent&)event;
			if (e.GetKeyCode() == SL_KEY_TAB)
				SL_TRACE("{0} is pressed | Tab", "Tab");
			SL_TRACE("{0} is pressed | (from char)", (char)e.GetKeyCode());
		}
	}
};

class SandBox :public SoLin::Application {
public:
	SandBox() {
		PushLayer(new ExampleLayer());
		//取消 PushOverlay(new SoLin::ImGuiLayer()); ，将其作为 SoLin 运行时 固定自动添加的图层
	}
	~SandBox() {

	}
};


SoLin::Application* SoLin::CreateApplication() {
	return new SandBox();
}