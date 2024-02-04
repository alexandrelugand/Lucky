#pragma once

#include "Base.h"
#include "Log.h"

#ifdef ENABLE_ASSERTS

	// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
	// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
	#define LK_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { LK##type##ERROR(msg, __VA_ARGS__); DEBUGBREAK(); } }
	#define LK_INTERNAL_ASSERT_WITH_MSG(type, check, ...) LK_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define LK_INTERNAL_ASSERT_NO_MSG(type, check) LK_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

	#define LK_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define LK_INTERNAL_ASSERT_GET_MACRO(...) EXPAND_MACRO( LK_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, LK_INTERNAL_ASSERT_WITH_MSG, LK_INTERNAL_ASSERT_NO_MSG) )

	// Currently accepts at least the condition and one additional parameter (the message) being optional
	#define LK_ASSERT(...) EXPAND_MACRO( LK_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
	#define LK_CORE_ASSERT(...) EXPAND_MACRO( LK_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
	#define LK_ASSERT(...)
	#define LK_CORE_ASSERT(...)
#endif
