#pragma once

#include <memory>

#ifdef PLATFORM_WINDOWS
    #ifdef _MSC_VER
        #define DEBUG_BREAK __debugbreak
    #else
        #define DEBUG_BREAK __builtin_trap()
    #endif
#else
	#define DEBUG_BREAK __builtin_trap()
#endif

#ifdef DEBUG
	#define ENABLE_ASSERTS
#endif

#ifdef ENABLE_ASSERTS

    #include "Log.h"
    
    #define LK_CORE_ASSERT_MESSAGE_INTERNAL(...)  ::Lucky::Log::PrintAssertMessage(::Lucky::Log::Type::Core, "Assertion Failed" __VA_OPT__(,) __VA_ARGS__)
    #define LK_ASSERT_MESSAGE_INTERNAL(...)  ::Lucky::Log::PrintAssertMessage(::Lucky::Log::Type::Client, "Assertion Failed" __VA_OPT__(,) __VA_ARGS__)

	#define LK_CORE_ASSERT(condition, ...) { if(!(condition)) { LK_CORE_ASSERT_MESSAGE_INTERNAL(__VA_ARGS__); DEBUG_BREAK; } }
	#define LK_ASSERT(condition, ...) { if(!(condition)) { LK_ASSERT_MESSAGE_INTERNAL(__VA_ARGS__); DEBUG_BREAK; } }
#else
	#define LK_CORE_ASSERT(condition, ...)
	#define LK_ASSERT(condition, ...)
#endif

#define BIT(x) (x << 1)
#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace Lucky
{
    template<typename T>
    using Scope = std::unique_ptr<T>;

    template<typename T>
    using Ref = std::shared_ptr<T>;

    inline std::string trim(std::string&& str)
    {
        str.erase(str.find_last_not_of(' ')+1);         //suffixing spaces
        str.erase(0, str.find_first_not_of(' '));       //prefixing spaces
        return str;
    }
} // namespace Lucky

