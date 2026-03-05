#include<SoLin.h>
#include<iostream>

class SandBox :public SoLin::Application {
public:
	SandBox() {

	}
	~SandBox() {

	}
};


SoLin::Application* SoLin::CreateApplication() {
	return new SandBox();
}