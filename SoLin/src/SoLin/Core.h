#pragma once

#ifdef SL_PLATFORM_WINDOWS
	#ifdef SL_BUILD_DLL
		#define SOLIN_API __declspec(dllexport)
	#else
		#define SOLIN_API __declspec(dllimport)
	#endif
#else
	#error SoLin only support Windows!
#endif

#ifdef SOLIN_ENABLE_ASSERTS
	//断言（如果x表示错误则语句运行，{0}占位的"__VA_ARGS__"代表"..."所输入的语句）
	#define SOLIN_CORE_ASSERT(x,...)\
		{#if(!x){\
			SOLIN_CORE_ERROR("Assertion Failed: {0}, __VA_ARGS__");\
			__debugbreak();}\
		}
	#define SOLIN_ASSERT(x,...)\
		{if(!x){\
			SOLIN_ERROR("Assertion Failed: {0}, __VA_ARGS__");\
			__debugbreak();}\
		}
#else
	#define SOLIN_CORE_ASSERT(x,...)
	#define SOLIN_ASSERT(x,...)
#endif

#define Bit(x) (1 << x)