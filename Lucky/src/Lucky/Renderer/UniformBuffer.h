#pragma once
#include "Shader.h"

namespace Lucky
{
	class UniformBuffer
	{
	public:
		virtual ~UniformBuffer() = default;
		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) = 0;

		static Ref<UniformBuffer> Create(const std::string& name, const Shader* shader, uint32_t binding, uint32_t size = 0);
	};
}
