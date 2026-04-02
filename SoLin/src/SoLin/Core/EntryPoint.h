#pragma once

#ifdef SL_PLATFORM_WINDOWS

extern SoLin::Application* SoLin::CreateApplication();

int main(int argc,char** argv) {

    //控制台输入输出均改为utf-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

	SoLin::Log::Init();
	SL_CORE_TRACE(u8"初始化日志");

    SL_PROFILE_BEGIN_SESSION("Startup", "SoLinProfile-Startup.json");
	auto app = SoLin::CreateApplication();
    SL_PROFILE_END_SESSION();

    SL_PROFILE_BEGIN_SESSION("Runtime", "SoLinProfile-Runtime.json");
	app->Run();
    SL_PROFILE_END_SESSION();

    SL_PROFILE_BEGIN_SESSION("Shutdown", "SoLinProfile-Shutdown.json");
	delete app;
    SL_PROFILE_END_SESSION();

}

#endif // SL_PLATFORM_WINDOWS
