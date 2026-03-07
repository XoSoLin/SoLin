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

#define Bit(x) (1 << x)