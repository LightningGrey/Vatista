#pragma once
#include "../spdlog/spdlog/spdlog.h"
#include "../spdlog/spdlog/fmt/ostr.h"
#include "../spdlog/spdlog/spdlog.h"


class Logger {
public:
	/*
		Initializes the logging subsystem, and sets up the color logger and debug trace utilities
	*/
	static void Init();

	/*
		De-initializes the logging subsytem, and cleans up all the logging resources
	 */
	static void Uninitialize();

	/*
		Gets the logging instance
	*/
	inline static std::shared_ptr<spdlog::logger>& GetLogger() { return myLogger; }
	/*
		Dumps the current stack trace into a string for logging
	*/
	static std::string DumpStackTrace();

private:
	static std::shared_ptr<spdlog::logger> myLogger;
};

// Client log macros
#define LOG_TRACE(...) ::Logger::GetLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)  ::Logger::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)  ::Logger::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) { ::Logger::GetLogger()->error(__VA_ARGS__); ::Logger::GetLogger()->error("Location: \n{}", ::Logger::DumpStackTrace()); }

// Allows us to assert if a value is true, and automagically debug break if it is false
#define LOG_ASSERT(x, ...) { if (!(x)) { ::Logger::GetLogger()->error(__VA_ARGS__); __debugbreak(); } }