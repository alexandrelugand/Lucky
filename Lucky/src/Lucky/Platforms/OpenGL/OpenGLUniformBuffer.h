#pragma once
#include "Lucky/Renderer/UniformBuffer.h"

namespace Lucky
{
	class OpenGLUniformBuffer : public UniformBuffer
	{
	public:
		OpenGLUniformBuffer(uint32_t binding, uint32_t size);
		virtual ~OpenGLUniformBuffer();

		void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;

	private:
		uint32_t m_UniformBufferId = 0;
	};
}
