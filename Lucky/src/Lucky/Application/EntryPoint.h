#pragma once

#ifdef __EMSCRIPTEN__
	#include <emscripten.h>
#else
	#define EMSCRIPTEN_KEEPALIVE
#endif
#include "Application.h"

extern Lucky::Application* CreateApplication();

EMSCRIPTEN_KEEPALIVE
extern int main(int argc, char** argv);