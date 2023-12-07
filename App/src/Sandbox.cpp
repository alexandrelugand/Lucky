#include "Sandbox.h"
#include "ExampleLayer.h"

SandBox::SandBox(const std::string& title) :
    Application(title)
{    
    LK_INFO("Application initialized");
    PushLayer(new ExampleLayer());
    PushOverlay(new Lucky::ImGuiLayer());
}

SandBox::~SandBox()
{
}

Lucky::Application* CreateApplication()
{
    return new SandBox("SandBox");
}