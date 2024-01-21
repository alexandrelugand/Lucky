#include "LuckyPch.h"
#include "OpenGLES3VertexBuffer.h"

namespace Lucky
{
	OpenGLES3VertexBuffer::OpenGLES3VertexBuffer(uint32_t size)
	{
		LK_PROFILE_FUNCTION();
		glGenBuffers(1, &m_VertexBufferId);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferId);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLES3VertexBuffer::OpenGLES3VertexBuffer(float *vertices, uint32_t size)
	{
		LK_PROFILE_FUNCTION();
		glGenBuffers(1, &m_VertexBufferId);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferId);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLES3VertexBuffer::~OpenGLES3VertexBuffer()
	{
		LK_PROFILE_FUNCTION();
		glDeleteBuffers(1, &m_VertexBufferId);
	}

	void OpenGLES3VertexBuffer::Bind() const
	{
		LK_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferId);
	}

	void OpenGLES3VertexBuffer::Unbind() const
	{
		LK_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLES3VertexBuffer::SetData(const void* data, uint32_t size)
	{
		LK_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferId);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}
}
