#include "LuckyEditorPch.h"
#include "Bootstrap.h"
#include "Lucky/EntryPoint.h"
#include "Lucky/Application/Application.h"

extern "C"
{
	EXPORT void STDCALL Bootstrap_Init()
	{
		Lucky::Log::Init();
		Lucky::Application* application = CreateApplication({ 0, nullptr });
		application->Run();
	}
}
