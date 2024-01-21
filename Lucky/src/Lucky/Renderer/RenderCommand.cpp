#include "LuckyPch.h"
#include "RenderCommand.h"

#ifndef __EMSCRIPTEN__	
#include "Lucky/Platforms/OpenGL/OpenGLRendererApi.h"
#else
#include "Lucky/Platforms/OpenGLES3/OpenGLES3RendererApi.h"
#endif

namespace Lucky
{
#ifndef __EMSCRIPTEN__	
	RendererApi* RenderCommand::s_RendererApi = new OpenGLRendererApi();
#else
	RendererApi* RenderCommand::s_RendererApi = new OpenGLES3RendererApi();
#endif
}
