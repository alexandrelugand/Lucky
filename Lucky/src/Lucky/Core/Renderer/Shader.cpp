#include "LuckyPch.h"

namespace Lucky
{
	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetApi())
		{
		case RendererApi::Api::None:
			LK_CORE_ASSERT(false, "None renderer API is not supported!");
			return nullptr;
		case RendererApi::Api::OpenGL:
			return new OpenGLShader(vertexSrc, fragmentSrc);
		}

		LK_CORE_ASSERT(false, "Unknown renderer API!");
		return nullptr;
	}
}

// namespace Lucky
