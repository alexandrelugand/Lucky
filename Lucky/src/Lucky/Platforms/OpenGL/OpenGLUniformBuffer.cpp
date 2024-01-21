#include "LuckyPch.h"
#include "OpenGLUniformBuffer.h"

namespace Lucky
{
	OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t binding, uint32_t size)
	{
		LK_PROFILE_FUNCTION();
		glGenBuffers(1, &m_UniformBufferId);
		glBindBuffer(GL_UNIFORM_BUFFER, m_UniformBufferId);
		glNamedBufferData(m_UniformBufferId, size, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_UniformBufferId);
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		LK_PROFILE_FUNCTION();
		glDeleteBuffers(1, &m_UniformBufferId);
	}

	void OpenGLUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
	{
		LK_PROFILE_FUNCTION();
		glNamedBufferSubData(m_UniformBufferId, offset, size, data);
	}
}
