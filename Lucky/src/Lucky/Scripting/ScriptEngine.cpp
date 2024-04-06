#include "LuckyPch.h"
#include "ScriptEngine.h"

#ifndef __EMSCRIPTEN__
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#endif

namespace Lucky
{
#ifndef __EMSCRIPTEN__
	struct ScriptEngineData
	{
		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;

		MonoAssembly* CoreAssembly = nullptr;
		MonoObject* Script = nullptr;
		MonoMethod* OnUpdate = nullptr;
	};

#else
	struct ScriptEngineData
	{
		uint64_t instance = 0;
	};
#endif
	static ScriptEngineData* s_Data;

	void ScriptEngine::Init()
	{
		s_Data = new ScriptEngineData();
		InitEngine();
	}

	void ScriptEngine::Shutdown()
	{
		ShutdownEngine();
		delete s_Data;
	}

#ifndef __EMSCRIPTEN__
	char* ReadBytes(const std::string& filePath, size_t* outSize)
	{
		std::ifstream stream(filePath, std::ios::binary | std::ios::ate);

		if(!stream)
		{
			// Failed to open the file
			return nullptr;
		}

		std::streampos end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		size_t size = end - stream.tellg();

		if(size == 0)
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
		size_t fileSize = 0;
		char* fileData = ReadBytes(assemblyPath, &fileSize);

		MonoImageOpenStatus status;
		MonoImage* image = mono_image_open_from_data_full(fileData, (uint32_t)fileSize, 1, &status, 0);

		if(status != MONO_IMAGE_OK)
		{
			const char* errorMessage = mono_image_strerror(status);
			LK_CORE_ERROR("Failed to load assembly: {0}", errorMessage);
			return nullptr;
		}

		MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
		mono_image_close(image);

		delete[] fileData;
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
#endif

	void ScriptEngine::InitEngine()
	{
#ifndef __EMSCRIPTEN__
		mono_set_assemblies_path("mono/lib");

		MonoDomain* rootDomain = mono_jit_init("LuckyJIRRuntime");
		LK_CORE_ASSERT(rootDomain);

		// Store the root domain pointer
		s_Data->RootDomain = rootDomain;

		 // Create an App Domain
		 s_Data->AppDomain = mono_domain_create_appdomain((char*)"LuckyScriptRuntime", nullptr);
		 mono_domain_set(s_Data->AppDomain, true);

		 s_Data->CoreAssembly = LoadCSharpAssembly("resources/Scripts/Lucky-ScriptCore.dll");
		 PrintAssemblyTypes(s_Data->CoreAssembly);

		 MonoImage* assemblyImage = mono_assembly_get_image(s_Data->CoreAssembly);
		 MonoClass* monoClass = mono_class_from_name(assemblyImage, "Lucky", "Main");

		 // 1. Create an object (and call constructor
		 MonoObject* instance = mono_object_new(s_Data->AppDomain, monoClass);
		 mono_runtime_object_init(instance);

		 // 2. Call function
		 MonoMethod* printMessageFunc = mono_class_get_method_from_name(monoClass, "PrintMessage", 0);
		 mono_runtime_invoke(printMessageFunc, instance, nullptr, nullptr);

		 // 3. Call function with param
		 MonoMethod* printIntFunc = mono_class_get_method_from_name(monoClass, "PrintInt", 1);

		 int value = 5;
		 void* param = &value;
		 mono_runtime_invoke(printIntFunc, instance, &param, nullptr);

		 MonoMethod* printIntsFunc = mono_class_get_method_from_name(monoClass, "PrintInts", 2);
		 int value2 = 508;
		 void* params[2] =
		 {
		 	&value,
		 	&value2
		 };
		 mono_runtime_invoke(printIntsFunc, instance, params, nullptr);

		 MonoString* monoString = mono_string_new(s_Data->AppDomain, "Hello world from C++!");
		 MonoMethod* printCustomMessageFunc = mono_class_get_method_from_name(monoClass, "PrintCustomMessage", 1);
		 void* stringParam = monoString;
		 mono_runtime_invoke(printCustomMessageFunc, instance, &stringParam, nullptr);
#else		
		EM_ASM(
			if (ScriptRuntime) {
				ScriptRuntime.PrintMessage();
			}
		);

		EM_ASM({
			if (ScriptRuntime) {
				ScriptRuntime.PrintInt($0);
			}
		}, 5);

		EM_ASM({
			if (ScriptRuntime) {
				ScriptRuntime.PrintInts($0, $1);
			}
		}, 1, 2);

		std::string message = "Hello from C++!";
		EM_ASM({
			if (ScriptRuntime) {
				let message = UTF8ToString($0);
				ScriptRuntime.PrintCustomMessage(message);
			}
		}, message.c_str());

		std::string filePath = "/resources/Scripts/Lucky-ScriptCore.dll";
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
			if(ScriptRuntime) {
				let assemblyName = UTF8ToString($0);
				ScriptRuntime.LoadLibrary(assemblyName, HEAPU8.subarray($1, $1 + $2));
			}
		}, assemblyName.c_str(), data, fsize);

		s_Data->instance = (uint64_t)EM_ASM_INT({
			if(ScriptRuntime) {
				let assemblyName = UTF8ToString($0);
				let className = UTF8ToString($1);
				let instance = ScriptRuntime.CreateInstance(assemblyName, className);
				console.log(instance);
				return instance;
			}
			return 0;
		}, (int)assemblyName.c_str(), (int)className.c_str());

		
#endif	
	}

	void ScriptEngine::ShutdownEngine()
	{
#ifndef __EMSCRIPTEN__
		mono_jit_cleanup(s_Data->RootDomain);
		s_Data->RootDomain = nullptr;
#endif		
	}

	void ScriptEngine::OnUpdate(Timestep ts)
	{
#ifndef __EMSCRIPTEN__
		if (s_Data->Script == nullptr)
		{
			MonoImage* assemblyImage = mono_assembly_get_image(s_Data->CoreAssembly);
			MonoClass* monoClass = mono_class_from_name(assemblyImage, "Lucky", "Main");

			s_Data->Script = mono_object_new(s_Data->AppDomain, monoClass);
			mono_runtime_object_init(s_Data->Script);

			s_Data->OnUpdate = mono_class_get_method_from_name(monoClass, "OnUpdate", 1);
		}
	
		double value = (double)(float)ts;
		void* param = &value;
		mono_runtime_invoke(s_Data->OnUpdate, s_Data->Script, &param, nullptr);
#else
		if(s_Data->instance == 0)
			return;

		std::string assemblyName = "Lucky-ScriptCore";
		std::string className = "Main";
		std::string methodName = "OnUpdate";

		EM_ASM({
			if(ScriptRuntime) {
				let assemblyName = UTF8ToString($0);
				let className = UTF8ToString($1);
				let methodName = UTF8ToString($2);
				let args = new Array();
				args[0] = $4;
				ScriptRuntime.Invoke(assemblyName, className, methodName, $3, args);
			}
		}, assemblyName.c_str(), className.c_str(), methodName.c_str(), s_Data->instance, (double)(float)ts);
#endif		
	}
}
