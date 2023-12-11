#pragma once

#include <stdint.h>

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

		inline const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; }
		inline const std::shared_ptr<IndexBuffer> GetIndexBuffer() const override { return m_IndexBuffer; }

		void AddVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer) override;
		void SetIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer) override;

	private:
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		uint32_t m_VertexArrayId;
	};

} // namespace Lucky
