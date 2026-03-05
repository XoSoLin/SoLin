#include<SoLin.h>
#include<iostream>

class SandBox :public SoLin::Application {
public:
	SandBox() {

	}
	~SandBox() {

	}
};

//void main() {
//	SandBox* sandbox = new SandBox();
//	sandbox->Run();
//	delete sandbox;
//	std::cin.get();
//}

SoLin::Application* SoLin::CreateApplication() {
	return new SandBox();
}