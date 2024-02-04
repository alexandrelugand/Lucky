#include "SandboxPch.h"
#include "Sandbox.h"

#include "Layers/Sandbox3D.h"
#include "Layers/Sandbox2D.h"

SandBox::SandBox(const Lucky::WindowProps& props, const Lucky::ApplicationSpecification& specification) :
    Application(props, specification)
{    
    //PushLayer(new Sandbox3D());
    PushLayer(new Sandbox2D());
    LK_INFO("Application initialized");
}

Lucky::Application* CreateApplication(const Lucky::ApplicationCommandLineArgs& args)
{
	Lucky::ApplicationSpecification spec;
	spec.WorkingDirectory = "../LuckyEditor";
	spec.CommandLineArgs = args;

    return new SandBox({"Lucky Sandbox", 1600, 900, false}, spec);
}
