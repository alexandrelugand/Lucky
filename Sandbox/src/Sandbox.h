#pragma once
#include <Lucky.h>

class SandBox : public Lucky::Application
{
public:
    SandBox(const Lucky::WindowProps& props);
    virtual ~SandBox();
};