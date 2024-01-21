#include "LuckyPch.h"
#include "RendererApi.h"

namespace Lucky
{
#ifndef __EMSCRIPTEN__
	RendererApi::Api RendererApi::s_Api = RendererApi::Api::OpenGL;
#else
	RendererApi::Api RendererApi::s_Api = RendererApi::Api::OpenGLES3;
#endif
}
