#include "LuckyPch.h"
#include "EntryPoint.h"

EMSCRIPTEN_KEEPALIVE
int main(int argc, char** argv)
{
    try
    {
		Lucky::Log::Init();

#ifdef __EMSCRIPTEN__    
		MAIN_THREAD_EM_ASM({
			let db = UTF8ToString($0);
			FS.mkdir(db);
			FS.mount(IDBFS, {}, db);

			FS.syncfs(true, function (err) {
				if(err != undefined)
					console.log("Mount IDBFS database failed. " + err);
			})
		}, ASSETS);
#endif

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
#ifdef __EMSCRIPTEN__
        emscripten_force_exit(EXIT_FAILURE);
#endif
        return -1;
    }
}
