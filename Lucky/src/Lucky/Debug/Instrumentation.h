#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <thread>

#define LK_PROFILE 0

/*
 * View profile with Chrome or Edge
 * chrome://tracing/
 * edge://tracing/
 */
namespace Lucky
{
	struct ProfileResult
	{
		std::string Name;
		long long Start, End;
		uint32_t ThreadID;
	};

	struct InstrumentationSession
	{
		std::string Name;
	};

	class Instrumentor
	{
	private:
		InstrumentationSession* m_CurrentSession;
		std::ofstream m_OutputStream;
		int m_ProfileCount;

	public:
		Instrumentor()
			: m_CurrentSession(nullptr), m_ProfileCount(0)
		{
		}

		void BeginSession(const std::string& name, const std::string& filepath = "results.json")
		{
			m_OutputStream.open(filepath);
			WriteHeader();
			m_CurrentSession = new InstrumentationSession{name};
		}

		void EndSession()
		{
			WriteFooter();
			m_OutputStream.close();
			delete m_CurrentSession;
			m_CurrentSession = nullptr;
			m_ProfileCount = 0;
		}

		void WriteProfile(const ProfileResult &result)
		{
			if (m_ProfileCount++ > 0)
				m_OutputStream << ",";

			std::string name = result.Name;
			std::replace(name.begin(), name.end(), '"', '\'');

			m_OutputStream << "{";
			m_OutputStream << "\"cat\":\"function\",";
			m_OutputStream << "\"dur\":" << (result.End - result.Start) << ',';
			m_OutputStream << "\"name\":\"" << name << "\",";
			m_OutputStream << "\"ph\":\"X\",";
			m_OutputStream << "\"pid\":0,";
			m_OutputStream << "\"tid\":" << result.ThreadID << ",";
			m_OutputStream << "\"ts\":" << result.Start;
			m_OutputStream << "}";

			m_OutputStream.flush();
		}

		void WriteHeader()
		{
			m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
			m_OutputStream.flush();
		}

		void WriteFooter()
		{
			m_OutputStream << "]}";
			m_OutputStream.flush();
		}

		static Instrumentor &Get()
		{
			static Instrumentor instance;
			return instance;
		}
	};

	template<typename... Args>
	class InstrumentationTimer
	{
	public:
		InstrumentationTimer(const char* name)
			: m_Name(name), m_Stopped(false)
		{
			m_StartTimepoint = std::chrono::high_resolution_clock::now();
		}

		~InstrumentationTimer()
		{
			if (!m_Stopped)
				Stop();
		}

		void Stop()
		{
			auto endTimepoint = std::chrono::high_resolution_clock::now();

			long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
			long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

			uint32_t threadID = (uint32_t)std::hash<std::thread::id>{}(std::this_thread::get_id());
			auto cdeclPos = m_Name.find("__cdecl ");
			if(cdeclPos != std::string::npos)
				m_Name.replace(cdeclPos, sizeof("__cdecl ") - 1, "");

			auto voidPos = m_Name.find("(void)");
			if (voidPos != std::string::npos)
				m_Name.replace(voidPos, sizeof("(void)") - 1, "()");

			Instrumentor::Get().WriteProfile({ m_Name, start, end, threadID });

			m_Stopped = true;
		}

	private:
		std::string m_Name;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
		bool m_Stopped;
	};
}

#if LK_PROFILE
	#define LK_PROFILE_BEGIN_SESSION(name, filepath) ::Lucky::Instrumentor::Get().BeginSession(name, filepath)
	#define LK_PROFILE_END_SESSION() ::Lucky::Instrumentor::Get().EndSession()
#define LK_PROFILE_SCOPE(name) ::Lucky::InstrumentationTimer timer##__LINE__(name)
	#ifdef COMPILER_MSVC
		#define LK_PROFILE_FUNCTION() LK_PROFILE_SCOPE(__FUNCSIG__)
	#else
		#define LK_PROFILE_FUNCTION() LK_PROFILE_SCOPE(__PRETTY_FUNCTION__)
	#endif
#else
	#define LK_PROFILE_BEGIN_SESSION(name, filepath)
	#define LK_PROFILE_END_SESSION()
	#define LK_PROFILE_SCOPE(name)
	#define LK_PROFILE_FUNCTION()
#endif
