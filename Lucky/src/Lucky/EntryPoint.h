#pragma once

#ifdef __EMSCRIPTEN__
	#include <emscripten.h>
#else
	#define EMSCRIPTEN_KEEPALIVE
#endif
#include "Lucky/Application/Application.h"

extern Lucky::Application* CreateApplication(const Lucky::ApplicationCommandLineArgs& args);

EMSCRIPTEN_KEEPALIVE
extern int main(int argc, char** argv);
