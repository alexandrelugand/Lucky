#include "LuckyPch.h"
#include "EntryPoint.h"

EMSCRIPTEN_KEEPALIVE
int main(int argc, char** argv)
{
    try
    {
		Lucky::Log::Init();

// #ifdef __EMSCRIPTEN__    
// 		EM_ASM(
// 			debugger;
// 			FS.mkdir('/data');
// 			FS.mount(IDBFS, {}, '/data');

// 			FS.syncfs(true, function (err) {
// 				console.log("syncfs:" + err);
// 			});
// 		);
// #endif

		LK_PROFILE_BEGIN_SESSION("Startup", "Startup.json");
		Lucky::Application* application = CreateApplication();
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
#ifdef __EMSCRIPTEN__
        emscripten_force_exit(EXIT_FAILURE);
#endif
        return -1;
    }
}
