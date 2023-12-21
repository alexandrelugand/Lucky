#include "LuckyPch.h"
#include "OpenGLVertexArray.h"

namespace Lucky
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float: 	return GL_FLOAT;
			case ShaderDataType::Float2: 	return GL_FLOAT;
			case ShaderDataType::Float3: 	return GL_FLOAT;
			case ShaderDataType::Float4: 	return GL_FLOAT;
			case ShaderDataType::Int: 		return GL_FLOAT;
			case ShaderDataType::Int2: 		return GL_INT;
			case ShaderDataType::Int3: 		return GL_INT;
			case ShaderDataType::Int4: 		return GL_INT;
			case ShaderDataType::Mat3: 		return GL_FLOAT;
			case ShaderDataType::Mat4: 		return GL_FLOAT;
			case ShaderDataType::Bool: 		return GL_BOOL;
		}

		LK_CORE_ASSERT(false, "Unknown ShaderDataType!")
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		LK_PROFILE_FUNCTION();
		glGenVertexArrays(1, &m_VertexArrayId);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		LK_PROFILE_FUNCTION();
		glDeleteVertexArrays(1, &m_VertexArrayId);
	}

	void OpenGLVertexArray::Bind() const
	{
		LK_PROFILE_FUNCTION();
		glBindVertexArray(m_VertexArrayId);
	}

	void OpenGLVertexArray::Unbind() const
	{
		LK_PROFILE_FUNCTION();
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(Ref<VertexBuffer> vertexBuffer)
	{
		LK_PROFILE_FUNCTION();
		glBindVertexArray(m_VertexArrayId);
		vertexBuffer->Bind();

#ifdef COMPILER_MSVC
#pragma warning( push, 0)
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
#endif
        uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
        for(const auto& element : layout)
        {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index,
                element.GetComponentCount(),
                ShaderDataTypeToOpenGLBaseType(element.Type),
                element.Normalized ? GL_TRUE : GL_FALSE, 
                layout.GetStride(), 
                (const void*) element.Offset);
            index++;
        }
#ifdef COMPILER_MSVC
#pragma warning(pop)
#else
#pragma GCC diagnostic pop
#endif

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(Ref<IndexBuffer> indexBuffer)
	{
		LK_PROFILE_FUNCTION();
		glBindVertexArray(m_VertexArrayId);
		indexBuffer->Bind();
		m_IndexBuffer = indexBuffer;
	}
} // namespace Lucky
