#include "LuckyPch.h"
#include "IndexBuffer.h"

#include "Renderer.h"
#include "RendererApi.h"
#ifndef __EMSCRIPTEN__
#include "Lucky/Platforms/OpenGL/OpenGLIndexBuffer.h"
#endif
#include "Lucky/Platforms/OpenGLES3/OpenGLES3IndexBuffer.h"

namespace Lucky
{
	Ref<IndexBuffer> IndexBuffer::Create(uint32_t *indices, uint32_t count)
	{
		switch (Renderer::GetApi())
		{
		case RendererApi::Api::None:
			LK_CORE_ASSERT(false, "None renderer API is not supported!");
			return nullptr;
#ifndef __EMSCRIPTEN__
		case RendererApi::Api::OpenGL:
			return CreateRef<OpenGLIndexBuffer>(indices, count);
#endif
		case RendererApi::Api::OpenGLES3:
			return CreateRef<OpenGLES3IndexBuffer>(indices, count);
		default:
			LK_CORE_ASSERT(false, "Renderer API is not supported!");
			return nullptr;
		}
	}
}

