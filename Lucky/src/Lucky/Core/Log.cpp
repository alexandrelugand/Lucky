#include "Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Lucky
{
    std::shared_ptr<spdlog::logger> Log::s_coreLogger;
    std::shared_ptr<spdlog::logger> Log::s_clientLogger;
    std::map<std::string, Log::TagDetails> s_EnabledTags;

    void Log::Init()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$", spdlog::pattern_time_type::local);
        s_coreLogger = spdlog::stdout_color_mt("LUCKY", spdlog::color_mode::automatic);
        s_coreLogger->set_level(spdlog::level::trace);

        s_clientLogger = spdlog::stdout_color_mt("APP", spdlog::color_mode::automatic);
        s_clientLogger->set_level(spdlog::level::trace);
    }

    void Log::Shutdown()
    {
        s_coreLogger.reset();
        s_clientLogger.reset();
    }    
}
