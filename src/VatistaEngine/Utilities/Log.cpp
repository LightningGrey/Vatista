#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include <DbgHelp.h>

namespace Vatista {
	std::shared_ptr<spdlog::logger> Log::engineLog;
	std::shared_ptr<spdlog::logger> Log::gameLog;

	//initialize
	void Log::Init()
	{
		//set output
		spdlog::set_pattern("%^[%T][%L] %n: %v%$");

		//trace logs
		engineLog = spdlog::stdout_color_mt("Vatista");
		engineLog->set_level(spdlog::level::trace);

		gameLog = spdlog::stdout_color_mt("Game");
		gameLog->set_level(spdlog::level::trace);

	}
	void Log::Uninitialize()
	{
		//dereference logs and shutdown
		engineLog = nullptr;
		gameLog = nullptr;
		spdlog::shutdown();
	}
}
