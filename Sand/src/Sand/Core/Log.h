#pragma once

#include "Sand/Core/Base.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Sand 
{

	class Log
	{
	public:
		static void Init();

		static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};

}

// Core log macros
#define SAND_CORE_TRACE(...)    ::Sand::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define SAND_CORE_INFO(...)     ::Sand::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SAND_CORE_WARN(...)     ::Sand::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SAND_CORE_ERROR(...)    ::Sand::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SAND_CORE_CRITICAL(...) ::Sand::Log::GetCoreLogger()->critical(__VA_ARGS__)