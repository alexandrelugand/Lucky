#pragma once
#include "Lucky/Renderer/UniformBuffer.h"
#include "Lucky/Renderer/Shader.h"

namespace Lucky
{
	class OpenGLES3UniformBuffer : public UniformBuffer
	{
	public:
		OpenGLES3UniformBuffer(const std::string& name, const Shader* shader, uint32_t binding);
		virtual ~OpenGLES3UniformBuffer();

		void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;

	private:
		uint32_t m_UniformBufferId = 0;
	};
}
