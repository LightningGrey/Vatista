#pragma once

#include <memory>
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Vatista {
	class Log {

	public:
		static void Init();
		static void Uninitialize();

		//logs engine actions
		inline static std::shared_ptr<spdlog::logger>& GetEngineLog() { return engineLog; };
		//logs game actions
		inline static std::shared_ptr<spdlog::logger>& GetGameLog() { return gameLog; };
	private:
		//logs engine actions 
		static std::shared_ptr<spdlog::logger> engineLog;
		//logs game actions
		static std::shared_ptr<spdlog::logger> gameLog;
	};

}

//engine log macros
#define VATISTA_LOG_TRACE(...)	::Vatista::Log::GetEngineLog()->trace(__VA_ARGS__);
#define VATISTA_LOG_INFO(...)	::Vatista::Log::GetEngineLog()->info(__VA_ARGS__);
#define VATISTA_LOG_WARN(...)	::Vatista::Log::GetEngineLog()->warn(__VA_ARGS__);
#define VATISTA_LOG_ERROR(...)	::Vatista::Log::GetEngineLog()->error(__VA_ARGS__);

//game log macros
#define GAME_LOG_TRACE(...)	::Vatista::Log::GetGameLog()->trace(__VA_ARGS__);
#define GAME_LOG_INFO(...)	::Vatista::Log::GetGameLog()->info(__VA_ARGS__);
#define GAME_LOG_WARN(...)	::Vatista::Log::GetGameLog()->warn(__VA_ARGS__);
#define GAME_LOG_ERROR(...)	::Vatista::Log::GetGameLog()->error(__VA_ARGS__);

