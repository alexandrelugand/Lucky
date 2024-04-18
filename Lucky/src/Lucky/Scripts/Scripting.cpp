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

	EXPORT void STDCALL LuckyScripting_LogVec2(glm::vec2* vector)
	{
		LK_CORE_INFO("Vector: X:{0}, Y:{1}", vector->x, vector->y);
	}

	EXPORT void STDCALL LuckyScripting_LogVec3(glm::vec3* vector)
	{
		LK_CORE_INFO("Vector: X:{0}, Y:{1}, Z:{2}", vector->x, vector->y, vector->z);
	}

	EXPORT void STDCALL LuckyScripting_LogVec4(glm::vec4* vector)
	{
		LK_CORE_INFO("Vector: X:{0}, Y:{1}, Z:{2}, W:{3}", vector->x, vector->y, vector->z, vector->w);
	}
}
