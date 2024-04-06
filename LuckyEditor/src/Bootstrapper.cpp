#include "Bootstrapper.h"
#include "Lucky/EntryPoint.h"
#include "Lucky/Application/Application.h"

Bootstrapper::Bootstrapper()
{
}

void Bootstrapper::Init()
{
	Lucky::Log::Init();
	Lucky::Application* application = CreateApplication({ 0, nullptr });
	application->Run();
}	
