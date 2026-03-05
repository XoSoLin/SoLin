#pragma once

#ifdef SL_PLATFORM_WINDOWS

extern SoLin::Application* SoLin::CreateApplication();

void main(int argc,char** argv) {
	auto app = SoLin::CreateApplication();
	app->Run();

	delete app;

}

#endif // SL_PLATFORM_WINDOWS
