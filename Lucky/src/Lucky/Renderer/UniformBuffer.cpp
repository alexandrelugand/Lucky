#include "LuckyPch.h"
#include "UniformBuffer.h"

#include "RendererApi.h"
#ifndef __EMSCRIPTEN__			
#include "Lucky/Platforms/OpenGL/OpenGLUniformBuffer.h"
#endif
#include "Lucky/Platforms/OpenGLES3/OpenGLES3UniformBuffer.h"

namespace Lucky
{
	Ref<UniformBuffer> UniformBuffer::Create(const std::string& name, const Shader* shader, uint32_t binding, uint32_t size)
	{
		switch (RendererApi::GetApi())
		{
		case RendererApi::Api::None:
			LK_CORE_ASSERT(false, "None renderer API is not supported!");
			return nullptr;
#ifndef __EMSCRIPTEN__			
		case RendererApi::Api::OpenGL:
			return CreateRef<OpenGLUniformBuffer>(binding, size);
#endif
		case RendererApi::Api::OpenGLES3:
			return CreateRef<OpenGLES3UniformBuffer>(name, shader, binding);
		default:
			LK_CORE_ASSERT(false, "Renderer API is not supported!");
			return nullptr;
		}
	}
}
