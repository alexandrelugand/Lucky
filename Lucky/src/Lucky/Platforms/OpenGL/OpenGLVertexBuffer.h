#pragma once

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

#include "Lucky/Core/Renderer/VertexBuffer.h"
#include "Lucky/Core/Renderer/VertexBufferLayout.h"

namespace Lucky
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		void Bind() const override;
		void Unbind() const override;

		inline const VertexBufferLayout& GetLayout() const override { return m_Layout; }
		inline void SetLayout(const VertexBufferLayout& layout) override { m_Layout = layout; }

	private:
		uint32_t m_VertexBufferId;
		VertexBufferLayout m_Layout;
	};
} // namespace Lucky
