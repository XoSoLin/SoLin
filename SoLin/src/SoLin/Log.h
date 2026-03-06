#pragma once

#include<memory>

#include"Core.h"
#include"spdlog/spdlog.h"

namespace SoLin {

	class SOLIN_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	public:
		
	};
}

// Core log macros
// 核心日志宏
#define SL_CORE_TRACE(...)		::SoLin::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define SL_CORE_INFO(...)		::SoLin::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SL_CORE_WARN(...)		::SoLin::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SL_CORE_ERROR(...)		::SoLin::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SL_CORE_FATAL(...)		::SoLin::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
// 客户端日志宏
#define SL_CLIENT_TRACE(...)	::SoLin::Log::GetClientLogger()->trace(__VA_ARGS__)
#define SL_CLIENT_INFO(...)		::SoLin::Log::GetClientLogger()->info(__VA_ARGS__)
#define SL_CLIENT_WARN(...)		::SoLin::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SL_CLIENT_ERROR(...)	::SoLin::Log::GetClientLogger()->error(__VA_ARGS__)
#define SL_CLIENT_FATAL(...)	::SoLin::Log::GetClientLogger()->fatal(__VA_ARGS__)

