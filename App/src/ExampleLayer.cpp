#include "ExampleLayer.h"

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

void ExampleLayer::OnEvent(Lucky::Event &event)
{
    LK_TRACE("{0}", event);
}
