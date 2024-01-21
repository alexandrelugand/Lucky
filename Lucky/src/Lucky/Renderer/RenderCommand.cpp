#include "LuckyPch.h"
#include "RenderCommand.h"

#ifndef __EMSCRIPTEN__	
#include "Lucky/Platforms/OpenGL/OpenGLRendererApi.h"
#endif
#include "Lucky/Platforms/OpenGLES3/OpenGLES3RendererApi.h"

namespace Lucky
{
#ifndef __EMSCRIPTEN__
	RendererApi* RenderCommand::s_RendererApi = RendererApi::GetApi() == RendererApi::Api::OpenGL ? (RendererApi*) new OpenGLRendererApi() : new OpenGLES3RendererApi();
#else
	RendererApi* RenderCommand::s_RendererApi = new OpenGLES3RendererApi();
#endif
}
