#include "SandboxPch.h"
#include <imgui.h>

ExampleLayer::ExampleLayer()
{
}

ExampleLayer::~ExampleLayer()
{
}

void ExampleLayer::OnUpdate()
{
    LK_TRACE("Example layer update");
}

void ExampleLayer::OnImGuiRender()
{
    ImGui::Begin("Test");
    ImGui::Text("Hello world!");
    ImGui::End();
}

void ExampleLayer::OnEvent(Lucky::Event &event)
{
    if(event.GetEventType() == Lucky::EventType::KeyPressed)
    {
        Lucky::KeyPressedEvent& e = (Lucky::KeyPressedEvent&)event;
        if(e.GetKeyCode() == LK_KEY_TAB)
            LK_TRACE("Tab key pressed");
        LK_TRACE("{0}", (char)e.GetKeyCode());
    }
}
