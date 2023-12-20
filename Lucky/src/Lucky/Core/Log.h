#pragma once

#include "Core.h"

// This ignores all warnings raised inside External headers
#ifdef COMPILER_MSVC
#pragma warning(push, 0)
#endif
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#ifdef COMPILER_MSVC
#pragma warning(pop)
#endif

#define LOG_LEVEL spdlog::level::info

namespace Lucky
{
    class Log
    {
    public:
        static void Init();
        static void Shutdown();

        static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

    private:
        static Ref<spdlog::logger> s_CoreLogger;
        static Ref<spdlog::logger> s_ClientLogger;
    };
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Core log macros
#define LK_CORE_TRACE(...)    ::Lucky::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define LK_CORE_INFO(...)     ::Lucky::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LK_CORE_WARN(...)     ::Lucky::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LK_CORE_ERROR(...)    ::Lucky::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LK_CORE_CRITICAL(...) ::Lucky::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define LK_TRACE(...)         ::Lucky::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LK_INFO(...)          ::Lucky::Log::GetClientLogger()->info(__VA_ARGS__)
#define LK_WARN(...)          ::Lucky::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LK_ERROR(...)         ::Lucky::Log::GetClientLogger()->error(__VA_ARGS__)
#define LK_CRITICAL(...)      ::Lucky::Log::GetClientLogger()->critical(__VA_ARGS__)