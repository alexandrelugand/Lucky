#include "LuckyPch.h"
#include "OpenGLVertexBuffer.h"

namespace Lucky
{
	OpenGLVertexBuffer::OpenGLVertexBuffer(float *vertices, uint32_t size)
	{
		LK_PROFILE_FUNCTION();
		glGenBuffers(1, &m_VertexBufferId);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferId);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		LK_PROFILE_FUNCTION();
		glDeleteBuffers(1, &m_VertexBufferId);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		LK_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferId);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		LK_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
} // namespace Lucky
