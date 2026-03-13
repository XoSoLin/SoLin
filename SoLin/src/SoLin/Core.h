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

#ifdef SL_DEBUG
	#define SOLIN_ENABLE_ASSERTS
#endif

#ifdef SOLIN_ENABLE_ASSERTS
	//断言（如果x表示错误则语句运行，{0}占位的"__VA_ARGS__"代表"..."所输入的语句）
	#define SL_CORE_ASSERT(x,...)\
		{if(!x){\
			SL_CORE_ERROR("Assertion Failed: {0}, __VA_ARGS__");\
			__debugbreak();}\
		}
	#define SL_ASSERT(x,...)\
		{if(!x){\
			SL_ERROR("Assertion Failed: {0}, __VA_ARGS__");\
			__debugbreak();}\
		}
#else
	#define SL_CORE_ASSERT(x,...)
	#define SL_ASSERT(x,...)
#endif

#define Bit(x) (1 << x)

#define SOLIN_BIND_EVENT_FN(fn) std::bind(&fn,this,std::placeholders::_1)