#pragma once

#include "Lucky/Renderer/VertexBuffer.h"
#include "Lucky/Renderer/VertexBufferLayout.h"

namespace Lucky
{
	class OpenGLES3VertexBuffer : public VertexBuffer
	{
	public:
		OpenGLES3VertexBuffer(uint32_t size);
		OpenGLES3VertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLES3VertexBuffer();

		void Bind() const override;
		void Unbind() const override;

		void SetData(const void* data, uint32_t size) override;

		inline const VertexBufferLayout& GetLayout() const override { return m_Layout; }
		inline void SetLayout(const VertexBufferLayout& layout) override { m_Layout = layout; }

	private:
		uint32_t m_VertexBufferId;
		VertexBufferLayout m_Layout;
	};
}
