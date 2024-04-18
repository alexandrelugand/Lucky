#pragma once
#include "Lucky/Core/Timestep.h"

namespace Lucky
{
	class ScriptEngine
	{
	public:
		static void Init();
		static void Shutdown();
		static void OnUpdate(Timestep ts);

	private:
		static void InitEngine();
		static void ShutdownEngine();
	};
}
