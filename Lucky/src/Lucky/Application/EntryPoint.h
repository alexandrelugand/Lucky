#pragma once
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE
#endif
#include <iostream>

#include "Application.h"

extern Lucky::Application* CreateApplication();

EMSCRIPTEN_KEEPALIVE
int main(int argc, char** argv)
{
    try
    {
        Lucky::Application* application = CreateApplication();
        application->Run();
        delete application;
        return 0;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
#ifdef __EMSCRIPTEN__
        emscripten_force_exit(EXIT_FAILURE);
#endif
        return -1;
    }
}