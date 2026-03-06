#pragma once

#ifdef SL_PLATFORM_WINDOWS

extern SoLin::Application* SoLin::CreateApplication();

void main(int argc,char** argv) {

	SoLin::Log::Init();
	SL_CORE_TRACE("初始化日志");
	int l = 164;
	SL_CLIENT_INFO("伊蕊 {0}cm",l);

	auto app = SoLin::CreateApplication();
	app->Run();

	delete app;

}

#endif // SL_PLATFORM_WINDOWS
