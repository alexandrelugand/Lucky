#include "LuckyPch.h"
#include "IndexBuffer.h"

namespace Lucky
{
	IndexBuffer* IndexBuffer::Create(uint32_t *indices, uint32_t count)
	{
		switch (Renderer::GetApi())
		{
		case RendererApi::None:
			LK_CORE_ASSERT(false, "None renderer API is not supported!");
			return nullptr;
		case RendererApi::OpenGL:
			return new OpenGLIndexBuffer(indices, count);
		}

		LK_CORE_ASSERT(false, "Unknown renderer API!");
		return nullptr;
	}
} // namespace Lucky

