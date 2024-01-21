#include "LuckyPch.h"
#include "RendererApi.h"

namespace Lucky
{
#ifdef OPENGL
	RendererApi::Api RendererApi::s_Api = RendererApi::Api::OpenGL;
#else
	RendererApi::Api RendererApi::s_Api = RendererApi::Api::OpenGLES3;
#endif
}
