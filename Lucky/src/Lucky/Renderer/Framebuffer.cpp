#include "LuckyPch.h"
#include "Framebuffer.h"

#include "Renderer.h"
#include "Lucky/Platforms/OpenGL/OpenGLFramebuffer.h"

namespace Lucky
{
	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetApi())
		{
		case RendererApi::Api::None:
			LK_CORE_ASSERT(false, "None renderer API is not supported!");
			return nullptr;
		case RendererApi::Api::OpenGL:
			return CreateRef<OpenGLFramebuffer>(spec);
		}

		LK_CORE_ASSERT(false, "Unknown renderer API!");
		return nullptr;
	}
}
