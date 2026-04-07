#include<SoLin.h>
#include<SoLin/Core/EntryPoint.h>

#include"EditorLayer.h"

class SoLinEditor :public SoLin::Application { 
public:
	SoLinEditor() {
		PushLayer(new EditorLayer());
	}
	~SoLinEditor() {

	}
};


SoLin::Application* SoLin::CreateApplication() {
	return new SoLinEditor();
}
