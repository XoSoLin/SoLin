#pragma once

#ifdef SL_PLATFORM_WINDOWS

extern SoLin::Application* SoLin::CreateApplication();

void main(int argc,char** argv) {

    //控制台输入输出均改为utf-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

	SoLin::Log::Init();
	SL_CORE_TRACE(u8"初始化日志");

	auto app = SoLin::CreateApplication();
	app->Run();

	delete app;

}

#endif // SL_PLATFORM_WINDOWS
