#include "LuckyPch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "RendererApi.h"
#include "Lucky/Platforms/OpenGL/OpenGLVertexArray.h"

namespace Lucky
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetApi())
		{
		case RendererApi::Api::None:
			LK_CORE_ASSERT(false, "None renderer API is not supported!");
			return nullptr;
		case RendererApi::Api::OpenGL:
			return std::make_shared<OpenGLVertexArray>();
		}

		LK_CORE_ASSERT(false, "Unknown renderer API!");
		return nullptr;
	}	
} // namespace Lucky

