#include "LuckyPch.h"
#include "Texture.h"

#include "Renderer.h"
#include "RendererApi.h"
#include "Lucky/Platforms/OpenGL/OpenGLTexture2D.h"

namespace Lucky
{
	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetApi())
		{
		case RendererApi::Api::None:
			LK_CORE_ASSERT(false, "None renderer API is not supported!");
			return nullptr;
		case RendererApi::Api::OpenGL:
			return CreateRef<OpenGLTexture2D>(width, height);
		}

		LK_CORE_ASSERT(false, "Unknown renderer API!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetApi())
		{
		case RendererApi::Api::None:
			LK_CORE_ASSERT(false, "None renderer API is not supported!");
			return nullptr;
		case RendererApi::Api::OpenGL:
			return CreateRef<OpenGLTexture2D>(path);
		}

		LK_CORE_ASSERT(false, "Unknown renderer API!");
		return nullptr;
	}
	
} // namespace Lucky
