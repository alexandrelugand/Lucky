#include "LuckyPch.h"
#include "VertexBuffer.h"

#include "Renderer.h"
#include "RendererApi.h"
#include "Lucky/Platforms/OpenGL/OpenGLVertexBuffer.h"

namespace Lucky
{
	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetApi())
		{
		case RendererApi::Api::None:
			LK_CORE_ASSERT(false, "None renderer API is not supported!");
			return nullptr;
		case RendererApi::Api::OpenGL:
			return CreateRef<OpenGLVertexBuffer>(size);
		}

		LK_CORE_ASSERT(false, "Unknown renderer API!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float *vertices, uint32_t size)
	{
		switch (Renderer::GetApi())
		{
		case RendererApi::Api::None:
			LK_CORE_ASSERT(false, "None renderer API is not supported!");
			return nullptr;
		case RendererApi::Api::OpenGL:
			return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}

		LK_CORE_ASSERT(false, "Unknown renderer API!");
		return nullptr;
	}
} // namespace Lucky

