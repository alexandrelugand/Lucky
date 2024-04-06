#include "LuckyPch.h"
#include "EntryPoint.h"

int main(int argc, char** argv)
{
    try
    {
		Lucky::Log::Init();

		LK_PROFILE_BEGIN_SESSION("Startup", "Startup.json");
		Lucky::Application* application = CreateApplication({argc, argv});
		LK_PROFILE_END_SESSION();

		LK_PROFILE_BEGIN_SESSION("Runtime", "Runtime.json");
		application->Run();
		LK_PROFILE_END_SESSION();

		LK_PROFILE_BEGIN_SESSION("Shutdown", "Shutdown.json");
		delete application;
		LK_PROFILE_END_SESSION();
		return 0;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}
