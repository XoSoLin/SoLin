#pragma once

#include<memory>

// 使用预定义宏进行平台检测
#ifdef _WIN32
	#ifdef _WIN64
		// Windows x64
		#define SL_PLATFORM_WINDOWS
	#else
		// Windows x86
		#error "x86 Builds are not supported!"
	#endif
#elif defined(__ANDROID__)
	// 我们还需要在__linux__之前检查__ANDROID__。因为android基于linux内核，并且它定义了__linux__
	#define SL_PLATFORM_ANDROID
	#error "Android is not supported!"
#elif defined(__Linux__)
	#define SL_PLATFORM_LINUX
	#error "Linux is not supported!"
#elif defined(__APPLE__) || defined(__MACH__)
	// Apple 有很多种形式，我们需要确定对应的平台，并进行对应的定义
	#include <TargetConditionals.h>	
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS Simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define SL_PLATFORM_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC
		#define SL_PLATFORM_MACOS
		#error "MacOS is not supported!"
	#else
		#error "Unknown Apple platform!"
	#endif
#else
	#error "Unknown platform!"
#endif

#ifdef SL_PLATFORM_WINDOWS
	#ifdef SL_DYNAMIC_LINK
		#ifdef SL_BUILD_DLL
			#define SOLIN_API __declspec(dllexport)
		#else
			#define SOLIN_API __declspec(dllimport)
		#endif
	#else
		#define SOLIN_API
	#endif
#else
	#error SoLin only support Windows!
#endif

//win情况下用宽字符来处理资源路径，其他均为utf-8保证跨平台性
//与std::filesystem::path一起使用
//TODO 后续或许可以封装一个自己的path类
#ifdef SL_PLATFORM_WINDOWS
    #define SLPATH(x) L##x
    #define SLOPEN(x) _wfopen(x.c_str(), L"rb")
#else
    #define SLPATH(x) x
    #define SLOPEN(x) fopen(x.c_str(),"rb")
#endif

#ifdef SL_DEBUG
	#define SOLIN_ENABLE_ASSERTS

    #ifdef SL_PLATFORM_WINDOWS
        #define SL_DEBUGBREAK() __debugbreak();
    #elif SL_PLATFORM_LINUX
        #include <signal.h>
        #define SL_DEBUGBREAK() raise(SIGTRAP)
    #else
        #error "Platform doesn't support debugbreak yet!"
    #endif
#else
    #define SL_DEBUGBREAK()
#endif

#ifdef SOLIN_ENABLE_ASSERTS
	//断言（如果x表示错误则语句运行，{0}占位的"__VA_ARGS__"代表"..."所输入的语句）
	#define SL_CORE_ASSERT(x,...)\
		{if(!x){\
			SL_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__);\
			SL_DEBUGBREAK();}\
		}
	#define SL_ASSERT(x,...)\
		{if(!x){\
			SL_ERROR("Assertion Failed: {0}", __VA_ARGS__);\
			SL_DEBUGBREAK();}\
		}
#else
	#define SL_CORE_ASSERT(x,...)
	#define SL_ASSERT(x,...)
#endif

#define Bit(x) (1 << x)

#define SOLIN_BIND_EVENT_FN(fn) std::bind(&fn,this,std::placeholders::_1)

namespace SoLin {
	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T,typename ...Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		// make_shared 的参数是 根据CreateRef所填参数 所动态调整的参数。
		// 这些来自CreateRef中的模版参数会被 std::forward 所完美转发
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T,typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}
}
