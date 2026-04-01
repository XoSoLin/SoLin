#pragma once

#ifdef SL_PLATFORM_WINDOWS

extern SoLin::Application* SoLin::CreateApplication();

void main(int argc,char** argv) {

	SoLin::Log::Init();
	SL_CORE_TRACE("初始化日志");
	int l = 164;
	SL_CLIENT_INFO("伊蕊 {0}cm",l);

	const char* filename = "测试纹理.png";
	for (int i = 0; filename[i]; i++) {
		printf("%02X ", (unsigned char)filename[i]);
	}

	auto app = SoLin::CreateApplication();
	app->Run();

	delete app;

}

#endif // SL_PLATFORM_WINDOWS
