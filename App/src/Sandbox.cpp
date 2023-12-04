#include "Sandbox.h"

SandBox::SandBox(const std::string& title) :
    Application(title)
{    
    LK_INFO("Application initialized");
}

SandBox::~SandBox()
{
}

Lucky::Application* CreateApplication()
{
    return new SandBox("SandBox");
}