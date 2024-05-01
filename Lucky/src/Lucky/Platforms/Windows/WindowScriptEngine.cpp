#include "LuckyPch.h"
#include "Lucky/Scripts/ScriptEngine.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

namespace Lucky
{
	struct ScriptEngineData
	{
		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;

		MonoAssembly* ApiAssembly = nullptr;
		MonoAssembly* CoreAssembly = nullptr;
		MonoObject* Script = nullptr;
		MonoMethod* OnUpdate = nullptr;
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

	char* ReadBytes(const std::string& filePath, size_t* outSize)
	{
		std::ifstream stream(filePath, std::ios::binary | std::ios::ate);

		if (!stream)
		{
			// Failed to open the file
			return nullptr;
		}

		std::streampos end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		size_t size = end - stream.tellg();

		if (size == 0)
		{
			// File is empty
			return nullptr;
		}

		char* buffer = new char[size];
		stream.read((char*)buffer, size);
		stream.close();

		*outSize = size;
		return buffer;
	}

	MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath)
	{
		MonoImageOpenStatus status;
		MonoImage* image = mono_image_open_full(assemblyPath.c_str(), &status, 0);
		if (status != MONO_IMAGE_OK)
		{
			const char* errorMessage = mono_image_strerror(status);
			LK_CORE_ERROR("Failed to load assembly: {0}", errorMessage);
			return nullptr;
		}

		MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
		mono_image_close(image);

		return assembly;
	}

	void PrintAssemblyTypes(MonoAssembly* assembly)
	{
		MonoImage* image = mono_assembly_get_image(assembly);
		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

			LK_CORE_INFO("{0}.{1}", nameSpace, name);
		}
	}

	void ScriptEngine::InitEngine()
	{
		mono_set_assemblies_path("mono/lib");

		MonoDomain* rootDomain = mono_jit_init("LuckyJIRRuntime");
		LK_CORE_ASSERT(rootDomain);

		// Store the root domain pointer
		s_ScriptingData->RootDomain = rootDomain;

		// Create an App Domain
		s_ScriptingData->AppDomain = mono_domain_create_appdomain((char*)"LuckyScriptRuntime", nullptr);
		mono_domain_set(s_ScriptingData->AppDomain, true);

		s_ScriptingData->ApiAssembly = mono_domain_assembly_open(s_ScriptingData->AppDomain, "resources/Scripts/Desktop/LuckyScripting.dll");
		s_ScriptingData->CoreAssembly = mono_domain_assembly_open(s_ScriptingData->AppDomain, "resources/Scripts/Desktop/Lucky-ScriptCore.dll");

		MonoImage* assemblyImage = mono_assembly_get_image(s_ScriptingData->CoreAssembly);
		MonoClass* monoClass = mono_class_from_name(assemblyImage, "Lucky", "Main");

		// 1. Create an object (and call constructor
		MonoObject* instance = mono_object_new(s_ScriptingData->AppDomain, monoClass);
		mono_runtime_object_init(instance);

		// 2. Call function
		MonoMethod* printMessageFunc = mono_class_get_method_from_name(monoClass, "PrintMessage", 0);
		mono_runtime_invoke(printMessageFunc, instance, nullptr, nullptr);

		//// 3. Call function with param
		//MonoMethod* printIntFunc = mono_class_get_method_from_name(monoClass, "PrintInt", 1);

		//int value = 5;
		//void* param = &value;
		//mono_runtime_invoke(printIntFunc, instance, &param, nullptr);

		//MonoMethod* printIntsFunc = mono_class_get_method_from_name(monoClass, "PrintInts", 2);
		//int value2 = 508;
		//void* params[2] =
		//{
		//	&value,
		//	&value2
		//};
		//mono_runtime_invoke(printIntsFunc, instance, params, nullptr);

		//MonoString* monoString = mono_string_new(s_ScriptingData->AppDomain, "Hello world from C++!");
		//MonoMethod* printCustomMessageFunc = mono_class_get_method_from_name(monoClass, "PrintCustomMessage", 1);
		//void* stringParam = monoString;
		//mono_runtime_invoke(printCustomMessageFunc, instance, &stringParam, nullptr);
	}

	void ScriptEngine::ShutdownEngine()
	{
		mono_jit_cleanup(s_ScriptingData->RootDomain);
		s_ScriptingData->RootDomain = nullptr;
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

		std::string assemblyName = "Lucky";

		if (s_ScriptingData->Script == nullptr)
		{
			MonoImage* scriptingAssemblyImage = mono_assembly_get_image(s_ScriptingData->ApiAssembly);
			MonoClass* monoBaseClass = mono_class_from_name(scriptingAssemblyImage, assemblyName.c_str(), "BaseScript");

			MonoImage* assemblyImage = mono_assembly_get_image(s_ScriptingData->CoreAssembly);
			MonoClass* monoClass = mono_class_from_name(assemblyImage, assemblyName.c_str(), className.c_str());

			s_ScriptingData->Script = mono_object_new(s_ScriptingData->AppDomain, monoClass);
			mono_runtime_object_init(s_ScriptingData->Script);

			s_ScriptingData->OnUpdate = mono_class_get_method_from_name(monoBaseClass, methodName.c_str(), 1);
		}

		mono_runtime_invoke(s_ScriptingData->OnUpdate, s_ScriptingData->Script, params, nullptr);
	}
}
