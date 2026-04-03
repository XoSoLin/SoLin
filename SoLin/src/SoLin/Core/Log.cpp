#include "slpch.h"
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"
#include<spdlog/sinks/basic_file_sink.h>

namespace SoLin {

	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init() {
        //LogSink（日志目标）:
        // 表示日志消息的输出方式或存储位置，并将其存储在智能指针 shared_ptr 中
        std::vector<spdlog::sink_ptr> logSinks;
        // 存储一个将日志消息输出到控制台，并带有颜色的日志目标
        logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
        // 指定一个日志目标，输出到"SoLin.log"，并在每次启动时清空文件(T 表示清空，F 表示追加写入)
        logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("SoLin.log", true));

        //样式设置
        logSinks[0]->set_pattern("%^[%T][%l] %n: %v%$");
        logSinks[1]->set_pattern("[%T] [%l] %n: %v");

        //创建日志记录器
        // 传入所有需要使用的日志目标
        s_CoreLogger = std::make_shared<spdlog::logger>("SoLin", begin(logSinks), end(logSinks));
        // 使用给定的名称注册给定的记录器
        spdlog::register_logger(s_CoreLogger);
        s_CoreLogger->set_level(spdlog::level::trace);
        // 记录日志消息时立即将缓冲区的日志刷新到输出目标,防止意外崩溃导致信息丢失
        s_CoreLogger->flush_on(spdlog::level::trace);

        s_ClientLogger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
        spdlog::register_logger(s_ClientLogger);
        s_ClientLogger->set_level(spdlog::level::trace);
        s_ClientLogger->flush_on(spdlog::level::trace);
	}
}
