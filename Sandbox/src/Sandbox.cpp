#include "SandboxPch.h"

SandBox::SandBox(const std::string& title) :
    Application(title)
{    
    LK_INFO("Application initialized");
    PushLayer(new ExampleLayer());
}

SandBox::~SandBox()
{
}

Lucky::Application* CreateApplication()
{
    return new SandBox("Lucky SandBox");
}