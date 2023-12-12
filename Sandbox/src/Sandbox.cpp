#include "SandboxPch.h"

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
    return new SandBox({"Lucky SandBox", 1280, 720, false});
}