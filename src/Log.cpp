#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Vatista {

	std::shared_ptr<spdlog::logger> Log::engineLog;
	std::shared_ptr<spdlog::logger> Log::gameLog;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T][%l] %n: %v");
		engineLog = spdlog::stdout_color_mt("Vatista");
		engineLog->set_level(spdlog::level::trace);

		gameLog = spdlog::stdout_color_mt("Game");
		gameLog->set_level(spdlog::level::trace);

	}
}
