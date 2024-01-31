#include "LuckyPch.h"
#include "OpenGLES3UniformBuffer.h"
#include "OpenGLES3Shader.h"

namespace Lucky
{
	OpenGLES3UniformBuffer::OpenGLES3UniformBuffer(const std::string& name, const Shader* shader, uint32_t binding)
	{
		LK_PROFILE_FUNCTION();

		GLint blockSize = 0;
		uint32_t programId = ((OpenGLES3Shader*)shader)->m_ProgramId;
		GLuint blockId = glGetUniformBlockIndex(programId, name.c_str());
		glUniformBlockBinding(programId, blockId, binding);
		glGetActiveUniformBlockiv(programId, blockId, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

		glGenBuffers(1, &m_UniformBufferId);
		glBindBuffer(GL_UNIFORM_BUFFER, m_UniformBufferId);
		glBufferData(GL_UNIFORM_BUFFER, blockSize, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_UniformBufferId);
	}

	OpenGLES3UniformBuffer::~OpenGLES3UniformBuffer()
	{
		LK_PROFILE_FUNCTION();
		glDeleteBuffers(1, &m_UniformBufferId);
	}

	void OpenGLES3UniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
	{
		LK_PROFILE_FUNCTION();
		glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	}
}
