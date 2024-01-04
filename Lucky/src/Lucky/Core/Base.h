#pragma once

#include <codecvt>
#include <memory>
#include <filesystem>

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
#define BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
#define STRCAT(x, y) std::string(x).append(y)

#ifdef __EMSCRIPTEN__
    #define ASSETS "assets_db"
#else
    #define ASSETS "assets"
#endif

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

    inline std::string Trim(std::string&& str)
    {
        str.erase(str.find_last_not_of(' ')+1);         //suffixing spaces
        str.erase(0, str.find_first_not_of(' '));       //prefixing spaces
        return str;
    }

	inline bool CreateDirectoryRecursive(const std::string& dirName, std::error_code& err)
	{
		err.clear();
        if (!std::filesystem::exists(dirName))
        {
            if (!std::filesystem::create_directories(dirName, err))
            {
                return false;
            }
        }
		return true;
	}

	inline std::string GetDirectoryName(const std::string& filePath)
    {
		auto p = std::filesystem::path(filePath);
		return p.parent_path().string();
    }

#ifdef COMPILER_MSVC
#pragma warning( push, 0)
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
	inline std::string ToNarrow(const wchar_t* str)
	{
		// ReSharper disable CppDeprecatedEntity
		return std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(str);
	}
#ifdef COMPILER_MSVC
#pragma warning(pop)
#else
#pragma GCC diagnostic pop
#endif

	inline std::wstring ToWide(const std::string& str)
	{
		return std::wstring(str.begin(), str.end());
	}
} // namespace Lucky

