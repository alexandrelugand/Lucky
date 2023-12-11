#include "LuckyPch.h"

namespace Lucky
{
	VertexBuffer* VertexBuffer::Create(float *vertices, uint32_t size)
	{
		switch (Renderer::GetApi())
		{
		case RendererApi::None:
			LK_CORE_ASSERT(false, "None renderer API is not supported!");
			return nullptr;
		case RendererApi::OpenGL:
			return new OpenGLVertexBuffer(vertices, size);
		}

		LK_CORE_ASSERT(false, "Unknown renderer API!");
		return nullptr;
	}
} // namespace Lucky

