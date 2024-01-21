#include "LuckyPch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "RendererApi.h"
#ifndef __EMSCRIPTEN__
#include "Lucky/Platforms/OpenGL/OpenGLVertexArray.h"
#else
#include "Lucky/Platforms/OpenGLES3/OpenGLES3VertexArray.h"
#endif

namespace Lucky
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetApi())
		{
		case RendererApi::Api::None:
			LK_CORE_ASSERT(false, "None renderer API is not supported!");
			return nullptr;
#ifndef __EMSCRIPTEN__
		case RendererApi::Api::OpenGL:
			return CreateRef<OpenGLVertexArray>();
#else
		case RendererApi::Api::OpenGLES3:
			return CreateRef<OpenGLES3VertexArray>();
#endif
		default:
			LK_CORE_ASSERT(false, "Renderer API is not supported!");
			return nullptr;
		}
	}	
}

