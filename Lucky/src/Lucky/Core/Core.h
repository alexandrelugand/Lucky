#pragma once

#ifdef PLATFORM_WINDOWS
    #ifdef _MSC_VER
        #define DEBUG_BREAK __debugbreak
    #else
        #define __builtin_trap
    #endif
#else
	#define DEBUG_BREAK __builtin_trap
#endif

// #ifdef __GNUG__
//     #define COMPILER_GCC
// #endif

#ifdef DEBUG
	#define ENABLE_ASSERTS
#endif

#ifdef ENABLE_ASSERTS
    
    #define CORE_ASSERT_MESSAGE_INTERNAL(...)  ::Lucky::Log::PrintAssertMessage(::Lucky::Log::Type::Core, "Assertion Failed" __VA_OPT__(,) __VA_ARGS__)
    #define ASSERT_MESSAGE_INTERNAL(...)  ::Lucky::Log::PrintAssertMessage(::Lucky::Log::Type::Client, "Assertion Failed" __VA_OPT__(,) __VA_ARGS__)

	#define CORE_ASSERT(condition, ...) { if(!(condition)) { CORE_ASSERT_MESSAGE_INTERNAL(__VA_ARGS__); DEBUG_BREAK; } }
	#define ASSERT(condition, ...) { if(!(condition)) { ASSERT_MESSAGE_INTERNAL(__VA_ARGS__); DEBUG_BREAK; } }
#else
	#define CORE_ASSERT(condition, ...)
	#define ASSERT(condition, ...)
#endif

#define BIT(x) (x << 1)
#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
