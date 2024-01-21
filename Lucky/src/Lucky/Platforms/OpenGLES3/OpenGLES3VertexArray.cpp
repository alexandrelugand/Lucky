#include "LuckyPch.h"
#include "OpenGLES3VertexArray.h"

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
			case ShaderDataType::Mat3: 		return GL_FLOAT;
			case ShaderDataType::Mat4: 		return GL_FLOAT;
			case ShaderDataType::Int: 		return GL_INT;
			case ShaderDataType::Int2: 		return GL_INT;
			case ShaderDataType::Int3: 		return GL_INT;
			case ShaderDataType::Int4: 		return GL_INT;
			case ShaderDataType::Bool: 		return GL_BOOL;
		}

		LK_CORE_ASSERT(false, "Unknown ShaderDataType!")
		return 0;
	}

	OpenGLES3VertexArray::OpenGLES3VertexArray()
	{
		LK_PROFILE_FUNCTION();
		glGenVertexArrays(1, &m_VertexArrayId);
	}

	OpenGLES3VertexArray::~OpenGLES3VertexArray()
	{
		LK_PROFILE_FUNCTION();
		glDeleteVertexArrays(1, &m_VertexArrayId);
	}

	void OpenGLES3VertexArray::Bind() const
	{
		LK_PROFILE_FUNCTION();
		glBindVertexArray(m_VertexArrayId);
	}

	void OpenGLES3VertexArray::Unbind() const
	{
		LK_PROFILE_FUNCTION();
		glBindVertexArray(0);
	}

	void OpenGLES3VertexArray::AddVertexBuffer(Ref<VertexBuffer> vertexBuffer)
	{
		LK_PROFILE_FUNCTION();
		LK_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

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
		for (const auto& element : layout)
		{
			switch (element.Type)
			{
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
			{
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(index,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.Type),
					element.Normalized ? GL_TRUE : GL_FALSE,
					layout.GetStride(),
					(const void*)element.Offset);
				index++;
				break;
			}
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
			case ShaderDataType::Bool:
			{
				glEnableVertexAttribArray(index);
				glVertexAttribIPointer(index,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.Type),
					layout.GetStride(),
					(const void*)element.Offset);
				index++;
				break;
			}
			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4:
			{
				uint8_t count = element.GetComponentCount();
				for (uint8_t i = 0; i < count; i++)
				{
					glEnableVertexAttribArray(index);
					glVertexAttribPointer(index,
						count,
						ShaderDataTypeToOpenGLBaseType(element.Type),
						element.Normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)(element.Offset + sizeof(float) * count * i));
					glVertexAttribDivisor(index, 1);
					index++;
				}
				break;
			}
			default:
				LK_CORE_ASSERT(false, "Unknown ShaderDataType!");
			}
		}
#ifdef COMPILER_MSVC
#pragma warning(pop)
#else
#pragma GCC diagnostic pop
#endif

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLES3VertexArray::SetIndexBuffer(Ref<IndexBuffer> indexBuffer)
	{
		LK_PROFILE_FUNCTION();
		glBindVertexArray(m_VertexArrayId);
		indexBuffer->Bind();
		m_IndexBuffer = indexBuffer;
	}
}
