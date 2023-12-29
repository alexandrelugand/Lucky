#include "LuckyPch.h"
#include "Shader.h"

#include "Renderer.h"
#include "RendererApi.h"
#include "Lucky/Platforms/OpenGL/OpenGLShader.h"

namespace Lucky
{
	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetApi())
		{
		case RendererApi::Api::None:
			LK_CORE_ASSERT(false, "None renderer API is not supported!");
			return nullptr;
		case RendererApi::Api::OpenGL:
			return CreateRef<OpenGLShader>(filepath);
		}

		LK_CORE_ASSERT(false, "Unknown renderer API!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetApi())
		{
		case RendererApi::Api::None:
			LK_CORE_ASSERT(false, "None renderer API is not supported!");
			return nullptr;
		case RendererApi::Api::OpenGL:
			return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}

		LK_CORE_ASSERT(false, "Unknown renderer API!");
		return nullptr;
	}
}

// namespace Lucky
