#include "LuckyPch.h"
#include "Texture.h"

#include "Renderer.h"
#include "RendererApi.h"
#include "Lucky/Platforms/OpenGL/OpenGLTexture2D.h"

namespace Lucky
{
	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetApi())
		{
		case RendererApi::Api::None:
			LK_CORE_ASSERT(false, "None renderer API is not supported!");
			return nullptr;
		case RendererApi::Api::OpenGL:
			return std::make_shared<OpenGLTexture2D>(path);
		}

		LK_CORE_ASSERT(false, "Unknown renderer API!");
		return nullptr;
	}
	
} // namespace Lucky
