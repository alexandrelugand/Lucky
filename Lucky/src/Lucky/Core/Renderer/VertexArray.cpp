#include "LuckyPch.h"

namespace Lucky
{
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetApi())
		{
		case RendererApi::Api::None:
			LK_CORE_ASSERT(false, "None renderer API is not supported!");
			return nullptr;
		case RendererApi::Api::OpenGL:
			return new OpenGLVertexArray();
		}

		LK_CORE_ASSERT(false, "Unknown renderer API!");
		return nullptr;
	}	
} // namespace Lucky

