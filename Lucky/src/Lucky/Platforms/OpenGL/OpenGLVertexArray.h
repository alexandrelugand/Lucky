#pragma once

#include "Lucky/Core/Renderer/VertexArray.h"

namespace Lucky
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		void Bind() const override;
		void Unbind() const override;

		inline const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; }
		inline const Ref<IndexBuffer> GetIndexBuffer() const override { return m_IndexBuffer; }

		void AddVertexBuffer(Ref<VertexBuffer> vertexBuffer) override;
		void SetIndexBuffer(Ref<IndexBuffer> indexBuffer) override;

	private:
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
		uint32_t m_VertexArrayId;
	};

} // namespace Lucky
