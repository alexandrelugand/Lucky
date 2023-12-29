#pragma once

#include <memory>

#ifdef PLATFORM_WINDOWS
    #ifdef _MSC_VER
        #define DEBUGBREAK __debugbreak
    #else
        #define DEBUGBREAK __builtin_trap
    #endif
#else
	#define DEBUGBREAK __builtin_trap
#endif

#if defined(__GNUG__)
	#define COMPILER_GPP
#elif defined(_MSC_VER)
	#define COMPILER_MSVC
#endif

#ifdef DEBUG
	#define ENABLE_ASSERTS
#endif

#define EXPAND_MACRO(x) x
#define STRINGIFY_MACRO(x) #x
#define BIT(x) (x << 1)
#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace Lucky
{
    template<typename T>
    using Scope = std::unique_ptr<T>;

    template<typename T, typename ...Args>
    constexpr Scope<T> CreateScope(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T, typename ...Args>
    constexpr Ref<T> CreateRef(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    inline std::string trim(std::string&& str)
    {
        str.erase(str.find_last_not_of(' ')+1);         //suffixing spaces
        str.erase(0, str.find_first_not_of(' '));       //prefixing spaces
        return str;
    }
} // namespace Lucky

