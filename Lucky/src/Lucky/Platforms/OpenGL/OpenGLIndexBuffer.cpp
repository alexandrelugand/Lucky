#include "LuckyPch.h"
#include "OpenGLIndexBuffer.h"

namespace Lucky
{
	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		: m_Count(count)
	{
		LK_PROFILE_FUNCTION();
		glGenBuffers(1, &m_IndexBufferId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		LK_PROFILE_FUNCTION();
		glDeleteBuffers(1, &m_IndexBufferId);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		LK_PROFILE_FUNCTION();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferId);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		LK_PROFILE_FUNCTION();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

} // namespace Lucky


