#include "LuckyPch.h"
#include "UniformBuffer.h"

#include "RendererApi.h"
#ifndef __EMSCRIPTEN__			
#include "Lucky/Platforms/OpenGL/OpenGLUniformBuffer.h"
#else
#include "Lucky/Platforms/OpenGLES3/OpenGLES3UniformBuffer.h"
#endif

namespace Lucky
{
	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		switch (RendererApi::GetApi())
		{
		case RendererApi::Api::None:
			LK_CORE_ASSERT(false, "None renderer API is not supported!");
			return nullptr;
#ifndef __EMSCRIPTEN__			
		case RendererApi::Api::OpenGL:
			return CreateRef<OpenGLUniformBuffer>(size, binding);
#endif
		default:
			LK_CORE_ASSERT(false, "Renderer API is not supported!");
			return nullptr;
		}
	}

	Ref<UniformBuffer> UniformBuffer::Create(const std::string& name, const Ref<Shader>& shader, uint32_t binding)
	{
		switch (RendererApi::GetApi())
		{
		case RendererApi::Api::None:
			LK_CORE_ASSERT(false, "None renderer API is not supported!");
			return nullptr;
#ifdef __EMSCRIPTEN__			
		case RendererApi::Api::OpenGLES3:
			return CreateRef<OpenGLES3UniformBuffer>(name, shader, binding);
#endif
		default:
			LK_CORE_ASSERT(false, "Renderer API is not supported!");
			return nullptr;
		}
	}
}
