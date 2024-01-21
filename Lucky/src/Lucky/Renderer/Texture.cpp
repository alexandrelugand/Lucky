#include "LuckyPch.h"
#include "Texture.h"

#include "Renderer.h"
#include "RendererApi.h"
#ifndef __EMSCRIPTEN__
#include "Lucky/Platforms/OpenGL/OpenGLTexture2D.h"
#else
#include "Lucky/Platforms/OpenGLES3/OpenGLES3Texture2D.h"
#endif

namespace Lucky
{
	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetApi())
		{
		case RendererApi::Api::None:
			LK_CORE_ASSERT(false, "None renderer API is not supported!");
			return nullptr;
#ifndef __EMSCRIPTEN__
		case RendererApi::Api::OpenGL:
			return CreateRef<OpenGLTexture2D>(width, height);
#else
		case RendererApi::Api::OpenGLES3:
			return CreateRef<OpenGLES3Texture2D>(width, height);
#endif
		default:
			LK_CORE_ASSERT(false, "Renderer API is not supported!");
			return nullptr;
		}
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetApi())
		{
		case RendererApi::Api::None:
			LK_CORE_ASSERT(false, "None renderer API is not supported!");
			return nullptr;
#ifndef __EMSCRIPTEN__
		case RendererApi::Api::OpenGL:
			return CreateRef<OpenGLTexture2D>(path);
#else
		case RendererApi::Api::OpenGLES3:
			return CreateRef<OpenGLES3Texture2D>(path);
#endif
		default:
			LK_CORE_ASSERT(false, "Renderer API is not supported!");
			return nullptr;
		}
	}
	
}
