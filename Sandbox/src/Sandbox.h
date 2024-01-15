#pragma once

#include <Lucky.h>
#include <Lucky/EntryPoint.h>

class SandBox : public Lucky::Application
{
public:
    SandBox(const Lucky::WindowProps& props, const Lucky::ApplicationCommandLineArgs& args);
    virtual ~SandBox();
};
