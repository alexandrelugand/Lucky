#include "LuckyPch.h"
#include "IndexBuffer.h"

#include "Renderer.h"
#include "RendererApi.h"
#include "Lucky/Platforms/OpenGL/OpenGLIndexBuffer.h"

namespace Lucky
{
	Ref<IndexBuffer> IndexBuffer::Create(uint32_t *indices, uint32_t count)
	{
		switch (Renderer::GetApi())
		{
		case RendererApi::Api::None:
			LK_CORE_ASSERT(false, "None renderer API is not supported!");
			return nullptr;
		case RendererApi::Api::OpenGL:
			return std::make_shared<OpenGLIndexBuffer>(indices, count);
		}

		LK_CORE_ASSERT(false, "Unknown renderer API!");
		return nullptr;
	}
} // namespace Lucky

