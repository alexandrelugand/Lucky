#include "LuckyPch.h"
#include "Framebuffer.h"

#include "Renderer.h"
#ifndef __EMSCRIPTEN__
#include "Lucky/Platforms/OpenGL/OpenGLFramebuffer.h"
#else
#include "Lucky/Platforms/OpenGLES3/OpenGLES3Framebuffer.h"
#endif

namespace Lucky
{
	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetApi())
		{
		case RendererApi::Api::None:
			LK_CORE_ASSERT(false, "None renderer API is not supported!");
			return nullptr;
#ifndef __EMSCRIPTEN__
		case RendererApi::Api::OpenGL:
			return CreateRef<OpenGLFramebuffer>(spec);
#else
		case RendererApi::Api::OpenGLES3:
			return CreateRef<OpenGLES3Framebuffer>(spec);
#endif
		default:
			LK_CORE_ASSERT(false, "Renderer API is not supported!");
			return nullptr;
		}
	}
}
