#pragma once

namespace Lucky
{
	class Platform
	{
	public:
		static std::string OpenFile(const char* filter, const std::string& folder = std::string());
		static std::string SaveFile(const char* filter, const std::string& folder = std::string());
	};

	class Time
	{
	public:
		static float GetTime();
	};
}
