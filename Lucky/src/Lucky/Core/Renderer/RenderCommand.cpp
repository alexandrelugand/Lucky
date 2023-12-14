#include "LuckyPch.h"
#include "RenderCommand.h"

#include "Lucky/Platforms/OpenGL/OpenGLRendererApi.h"

namespace Lucky
{
	RendererApi* RenderCommand::s_RendererApi = new OpenGLRendererApi();
} // namespace Lucky
