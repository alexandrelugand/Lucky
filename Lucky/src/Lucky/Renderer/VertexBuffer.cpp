#include "LuckyPch.h"
#include "VertexBuffer.h"

#include "Renderer.h"
#include "RendererApi.h"
#ifndef __EMSCRIPTEN__
#include "Lucky/Platforms/OpenGL/OpenGLVertexBuffer.h"
#else
#include "Lucky/Platforms/OpenGLES3/OpenGLES3VertexBuffer.h"
#endif

namespace Lucky
{
	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetApi())
		{
		case RendererApi::Api::None:
			LK_CORE_ASSERT(false, "None renderer API is not supported!");
			return nullptr;
#ifndef __EMSCRIPTEN__
		case RendererApi::Api::OpenGL:
			return CreateRef<OpenGLVertexBuffer>(size);
#else
		case RendererApi::Api::OpenGLES3:
			return CreateRef<OpenGLES3VertexBuffer>(size);
#endif
		default:
			LK_CORE_ASSERT(false, "Renderer API is not supported!");
			return nullptr;
		}
	}

	Ref<VertexBuffer> VertexBuffer::Create(float *vertices, uint32_t size)
	{
		switch (Renderer::GetApi())
		{
		case RendererApi::Api::None:
			LK_CORE_ASSERT(false, "None renderer API is not supported!");
			return nullptr;
#ifndef __EMSCRIPTEN__
		case RendererApi::Api::OpenGL:
			return CreateRef<OpenGLVertexBuffer>(vertices, size);
#else
		case RendererApi::Api::OpenGLES3:
			return CreateRef<OpenGLES3VertexBuffer>(vertices, size);
#endif
		default:
			LK_CORE_ASSERT(false, "Renderer API is not supported!");
			return nullptr;
		}
	}
}

