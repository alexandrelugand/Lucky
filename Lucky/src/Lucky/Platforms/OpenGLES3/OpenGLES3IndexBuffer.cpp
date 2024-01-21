#include "LuckyPch.h"
#include "OpenGLES3IndexBuffer.h"

namespace Lucky
{
	OpenGLES3IndexBuffer::OpenGLES3IndexBuffer(uint32_t* indices, uint32_t count)
		: m_Count(count)
	{
		LK_PROFILE_FUNCTION();
		glGenBuffers(1, &m_IndexBufferId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLES3IndexBuffer::~OpenGLES3IndexBuffer()
	{
		LK_PROFILE_FUNCTION();
		glDeleteBuffers(1, &m_IndexBufferId);
	}

	void OpenGLES3IndexBuffer::Bind() const
	{
		LK_PROFILE_FUNCTION();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferId);
	}

	void OpenGLES3IndexBuffer::Unbind() const
	{
		LK_PROFILE_FUNCTION();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}


