#include "LuckyApi.h"
#include "Lucky/Core/Log.h"

void LuckyApi::Log(const char* message)
{
	const std::string str(message);
	LK_CORE_INFO(str);
}
