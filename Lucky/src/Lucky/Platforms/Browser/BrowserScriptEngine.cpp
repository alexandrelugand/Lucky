#include "LuckyPch.h"
#include "Lucky/Scripts/ScriptEngine.h"

namespace Lucky
{
	struct ScriptEngineData
	{
		uint64_t instance = 0;
	};

	static ScriptEngineData* s_ScriptingData;

	void ScriptEngine::Init()
	{
		s_ScriptingData = new ScriptEngineData();
		InitEngine();
	}

	void ScriptEngine::Shutdown()
	{
		ShutdownEngine();
		delete s_ScriptingData;
	}

	void ScriptEngine::InitEngine()
	{
		std::string filePath = "/resources/Scripts/Browser/Lucky-ScriptCore.dll";
		FILE* f = fopen(filePath.c_str(), "rb");
		fseek(f, 0, SEEK_END);
		long fsize = ftell(f);
		fseek(f, 0, SEEK_SET);

		char* data = (char*)malloc(fsize + 1);
		fread(data, fsize, 1, f);
		fclose(f);

		std::string assemblyName = "Lucky-ScriptCore";
		std::string className = "Main";

		EM_ASM({
			let assemblyName = UTF8ToString($0);
			Scripting.LoadLibrary(assemblyName, HEAPU8.subarray($1, $1 + $2));
			}, assemblyName.c_str(), data, fsize);

		s_ScriptingData->instance = (uint64_t)EM_ASM_INT({
			let assemblyName = UTF8ToString($0);
			let className = UTF8ToString($1);
			let instance = Scripting.CreateInstance(assemblyName, className);
			return instance;
			}, assemblyName.c_str(), className.c_str());

		std::string methodName = "PrintMessage";
		EM_ASM({
			let assemblyName = UTF8ToString($0);
			let className = UTF8ToString($1);
			let methodName = UTF8ToString($2);
			//let args = new Array();
			Scripting.Invoke(assemblyName, className, methodName, $3/*, args*/);
			}, assemblyName.c_str(), className.c_str(), methodName.c_str(), s_ScriptingData->instance);
	}

	void ScriptEngine::ShutdownEngine()
	{
	}

	void ScriptEngine::OnUpdate(Timestep ts)
	{
		std::string methodName = "OnUpdateInternal";

		struct OnUpdateArgs
		{
			void* methodNamePtr;
			float ts;
		};

		OnUpdateArgs args;
		args.methodNamePtr = (void*)methodName.c_str();
		args.ts = ts;

		void* params[1] =
		{
			&args
		};

		std::string className = "Main";

		if (s_ScriptingData->instance == 0)
			return;

		std::string assemblyName = "Lucky-ScriptCore";

		EM_ASM({
			let assemblyName = UTF8ToString($0);
			let className = UTF8ToString($1);
			let methodName = UTF8ToString($2);
			let instance = $3;
			let args = Array.from(new Uint32Array(Module.HEAP8.buffer, $4, $5));
			Scripting.Invoke(assemblyName, className, methodName, instance, args);
			}, assemblyName.c_str(), className.c_str(), methodName.c_str(), s_ScriptingData->instance, params, sizeof(params) / sizeof(void*));
	}
}
