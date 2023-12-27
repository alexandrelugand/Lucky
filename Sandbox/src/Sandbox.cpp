#include "SandboxPch.h"
#include "Sandbox.h"

#include "Layers/Sandbox3D.h"
#include "Layers/Sandbox2D.h"

SandBox::SandBox(const Lucky::WindowProps& props) :
    Application(props)
{    
    //PushLayer(new Sandbox3D());
    PushLayer(new Sandbox2D());
    LK_INFO("Application initialized");
}

SandBox::~SandBox()
{
}

Lucky::Application* CreateApplication()
{
    return new SandBox({"Lucky Sandbox", 1280, 720, false});
}
