#include "SandboxPch.h"
#include "Sandbox.h"

#include "Layers/Sandbox3D.h"
#include "Layers/Sandbox2D.h"

SandBox::SandBox(const Lucky::WindowProps& props, const Lucky::ApplicationCommandLineArgs& args) :
    Application(props, args)
{    
    //PushLayer(new Sandbox3D());
    PushLayer(new Sandbox2D());
    LK_INFO("Application initialized");
}

SandBox::~SandBox()
{
}

Lucky::Application* CreateApplication(const Lucky::ApplicationCommandLineArgs& args)
{
    return new SandBox({"Lucky Sandbox", 1600, 900, false}, args);
}
