#pragma once

#include <map>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/fmt.h>
#include <spdlog/fmt/ostr.h>

#ifndef __EMSCRIPTEN__
#define ASSERT_MESSAGE_BOX (!DIST && PLATFORM_WINDOWS)
#endif

#ifdef ASSERT_MESSAGE_BOX
	#ifdef PLATFORM_WINDOWS
		#include <Windows.h>
	#endif
#endif

namespace Lucky
{
    class Log
    {
    public:
        enum class Type : uint8_t
		{
			Core = 0,
            Client = 1
		};

		enum class Level : uint8_t
		{
			Trace = 0,
            Info, 
            Warn, 
            Error, 
            Fatal
		};

        struct TagDetails
		{
			bool Enabled = true;
			Level LevelFilter = Level::Trace;
		};

        static void Init();
        static void Shutdown();

        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_coreLogger; }
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_clientLogger; }

        static bool HasTag(const std::string& tag) { return s_EnabledTags.find(tag) != s_EnabledTags.end(); }
		static std::map<std::string, TagDetails>& EnabledTags() { return s_EnabledTags; }

        template<typename... Args>
		static void PrintMessage(Type type, Level level, std::string_view tag, Args&&... args);

		template<typename... Args>
		static void PrintAssertMessage(Type type, std::string_view prefix, Args&&... args);

    private:
        static std::shared_ptr<spdlog::logger> s_coreLogger;
        static std::shared_ptr<spdlog::logger> s_clientLogger;
        inline static std::map<std::string, TagDetails> s_EnabledTags;
    };

    template<typename... Args>
	void Log::PrintMessage(Log::Type type, Log::Level level, std::string_view tag, Args&&... args)
	{
		auto detail = s_EnabledTags[std::string(tag)];
		if (detail.Enabled && detail.LevelFilter <= level)
		{
			auto logger = (type == Type::Core) ? GetCoreLogger() : GetClientLogger();
			std::string logString = tag.empty() ? "{0}{1}" : "[{0}] {1}";
            
			switch (level)
			{
			case Level::Trace:
				logger->trace(logString, tag, fmt::format(std::forward<Args>(args)...));
				break;
			case Level::Info:
				logger->info(logString, tag, fmt::format(std::forward<Args>(args)...));
				break;
			case Level::Warn:
				logger->warn(logString, tag, fmt::format(std::forward<Args>(args)...));
				break;
			case Level::Error:
				logger->error(logString, tag, fmt::format(std::forward<Args>(args)...));
				break;
			case Level::Fatal:
				logger->critical(logString, tag, fmt::format(std::forward<Args>(args)...));
				break;
			}
		}
	}

	template<typename... Args>
	void Log::PrintAssertMessage(Log::Type type, std::string_view prefix, Args&&... args)
	{
		auto logger = (type == Type::Core) ? GetCoreLogger() : GetClientLogger();
		logger->error("{0}: {1}", prefix, fmt::format(std::forward<Args>(args)...));

#ifdef ASSERT_MESSAGE_BOX
		std::string message = fmt::format(std::forward<Args>(args)...);
		MessageBoxA(nullptr, message.c_str(), "Lucky Assert", MB_OK | MB_ICONERROR);
#endif
	}

    template<>
	inline void Log::PrintAssertMessage(Log::Type type, std::string_view prefix)
	{
		auto logger = (type == Type::Core) ? GetCoreLogger() : GetClientLogger();
		logger->error("{0}", prefix);
#ifdef ASSERT_MESSAGE_BOX
		MessageBoxA(nullptr, "No message :(", "Lucky Assert", MB_OK | MB_ICONERROR);
#endif
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Tagged logs (prefer these!) 

// Core logging
#define LK_CORE_TRACE_TAG(tag, ...) ::Lucky::Log::PrintMessage(::Lucky::Log::Type::Core, ::Lucky::Log::Level::Trace, tag, __VA_ARGS__)
#define LK_CORE_INFO_TAG(tag, ...)  ::Lucky::Log::PrintMessage(::Lucky::Log::Type::Core, ::Lucky::Log::Level::Info, tag, __VA_ARGS__)
#define LK_CORE_WARN_TAG(tag, ...)  ::Lucky::Log::PrintMessage(::Lucky::Log::Type::Core, ::Lucky::Log::Level::Warn, tag, __VA_ARGS__)
#define LK_CORE_ERROR_TAG(tag, ...) ::Lucky::Log::PrintMessage(::Lucky::Log::Type::Core, ::Lucky::Log::Level::Error, tag, __VA_ARGS__)
#define LK_CORE_FATAL_TAG(tag, ...) ::Lucky::Log::PrintMessage(::Lucky::Log::Type::Core, ::Lucky::Log::Level::Fatal, tag, __VA_ARGS__)

// Client logging
#define LK_TRACE_TAG(tag, ...) ::Lucky::Log::PrintMessage(::Lucky::Log::Type::Client, ::Lucky::Log::Level::Trace, tag, __VA_ARGS__)
#define LK_INFO_TAG(tag, ...)  ::Lucky::Log::PrintMessage(::Lucky::Log::Type::Client, ::Lucky::Log::Level::Info, tag, __VA_ARGS__)
#define LK_WARN_TAG(tag, ...)  ::Lucky::Log::PrintMessage(::Lucky::Log::Type::Client, ::Lucky::Log::Level::Warn, tag, __VA_ARGS__)
#define LK_ERROR_TAG(tag, ...) ::Lucky::Log::PrintMessage(::Lucky::Log::Type::Client, ::Lucky::Log::Level::Error, tag, __VA_ARGS__)
#define LK_FATAL_TAG(tag, ...) ::Lucky::Log::PrintMessage(::Lucky::Log::Type::Client, ::Lucky::Log::Level::Fatal, tag, __VA_ARGS__)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Core Logging
#define LK_CORE_TRACE(...)  ::Lucky::Log::PrintMessage(::Lucky::Log::Type::Core, ::Lucky::Log::Level::Trace, "", __VA_ARGS__)
#define LK_CORE_INFO(...)   ::Lucky::Log::PrintMessage(::Lucky::Log::Type::Core, ::Lucky::Log::Level::Info, "", __VA_ARGS__)
#define LK_CORE_WARN(...)   ::Lucky::Log::PrintMessage(::Lucky::Log::Type::Core, ::Lucky::Log::Level::Warn, "", __VA_ARGS__)
#define LK_CORE_ERROR(...)  ::Lucky::Log::PrintMessage(::Lucky::Log::Type::Core, ::Lucky::Log::Level::Error, "", __VA_ARGS__)
#define LK_CORE_FATAL(...)  ::Lucky::Log::PrintMessage(::Lucky::Log::Type::Core, ::Lucky::Log::Level::Fatal, "", __VA_ARGS__)

// Client Logging
#define LK_TRACE(...)   ::Lucky::Log::PrintMessage(::Lucky::Log::Type::Client, ::Lucky::Log::Level::Trace, "", __VA_ARGS__)
#define LK_INFO(...)    ::Lucky::Log::PrintMessage(::Lucky::Log::Type::Client, ::Lucky::Log::Level::Info, "", __VA_ARGS__)
#define LK_WARN(...)    ::Lucky::Log::PrintMessage(::Lucky::Log::Type::Client, ::Lucky::Log::Level::Warn, "", __VA_ARGS__)
#define LK_ERROR(...)   ::Lucky::Log::PrintMessage(::Lucky::Log::Type::Client, ::Lucky::Log::Level::Error, "", __VA_ARGS__)
#define LK_FATAL(...)   ::Lucky::Log::PrintMessage(::Lucky::Log::Type::Client, ::Lucky::Log::Level::Fatal, "", __VA_ARGS__)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define LK_CORE_LOG_ERROR(...) ::Lucky::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LK_CORE_LOG_WARN(...) ::Lucky::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LK_CORE_LOG_INFO(...) ::Lucky::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LK_CORE_LOG_TRACE(...) ::Lucky::Log::GetCoreLogger()->trace(__VA_ARGS__)

#define LK_LOG_ERROR(...) ::Lucky::Log::GetClientLogger()->error(__VA_ARGS__)
#define LK_LOG_WARN(...) ::Lucky::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LK_LOG_INFO(...) ::Lucky::Log::GetClientLogger()->info(__VA_ARGS__)
#define LK_LOG_TRACE(...) ::Lucky::Log::GetClientLogger()->trace(__VA_ARGS__)