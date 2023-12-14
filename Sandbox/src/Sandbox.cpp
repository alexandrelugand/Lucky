#include "SandboxPch.h"
#include "Sandbox.h"

#include "ExampleLayer.h"

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

    return new SandBox({"Lucky Sandbox", 1280, 720, false});
}