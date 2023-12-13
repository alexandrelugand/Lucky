#include "SandboxPch.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

SandBox::SandBox(const Lucky::WindowProps& props) :
    Application(props)
{    
    LK_INFO("Application initialized");
    PushLayer(new ExampleLayer());
}

SandBox::~SandBox()
{
}

Lucky::Application* CreateApplication()
{
// #ifdef __EMSCRIPTEN__    
//      EM_ASM(
//         FS.mkdir('/data');
//         FS.mount(IDBFS, {}, '/data');

//         FS.syncfs(true, function (err) {
//             console.log("syncfs");
//         });
//     );
// #endif

    return new SandBox({"Lucky SandBox", 1280, 720, false});
}