#include "LuckyPch.h"
#include "Scripting.h"
#include "Lucky/Core/Log.h"

extern "C"
{
	EXPORT void STDCALL LuckyScripting_Info(const char* str)
	{
		const std::string log(str);
		LK_CORE_INFO(log);
	}

	EXPORT void STDCALL LuckyScripting_Warn(const char* str)
	{
		const std::string log(str);
		LK_CORE_WARN(log);
	}

	EXPORT void STDCALL LuckyScripting_Error(const char* str)
	{
		const std::string log(str);
		LK_CORE_ERROR(log);
	}
}
