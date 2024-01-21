#include "LuckyPch.h"
#include "Shader.h"

#include "Renderer.h"
#include "RendererApi.h"
#ifndef __EMSCRIPTEN__
#include "Lucky/Platforms/OpenGL/OpenGLShader.h"
#endif
#include "Lucky/Platforms/OpenGLES3/OpenGLES3Shader.h"

namespace Lucky
{
	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetApi())
		{
		case RendererApi::Api::None:
			LK_CORE_ASSERT(false, "None renderer API is not supported!");
			return nullptr;
#ifndef __EMSCRIPTEN__
		case RendererApi::Api::OpenGL:
			return CreateRef<OpenGLShader>(filepath);
#endif
		case RendererApi::Api::OpenGLES3:
			return CreateRef<OpenGLES3Shader>(filepath);
		default:
			LK_CORE_ASSERT(false, "Renderer API is not supported!");
			return nullptr;
		}
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetApi())
		{
		case RendererApi::Api::None:
			LK_CORE_ASSERT(false, "None renderer API is not supported!");
			return nullptr;
#ifndef __EMSCRIPTEN__
		case RendererApi::Api::OpenGL:
			return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
#endif
		case RendererApi::Api::OpenGLES3:
			return CreateRef<OpenGLES3Shader>(name, vertexSrc, fragmentSrc);
		default:
			LK_CORE_ASSERT(false, "Renderer API is not supported!");
			return nullptr;
		}
	}
}

// namespace Lucky
