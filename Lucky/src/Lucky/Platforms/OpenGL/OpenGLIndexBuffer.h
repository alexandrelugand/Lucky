#pragma once 

#include "Lucky/Core/Renderer/IndexBuffer.h"

namespace Lucky
{
	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		void Bind() const override;
		void Unbind() const override;
		inline uint32_t GetCount() const override { return m_Count; }

	private:
		uint32_t m_IndexBufferId;
		uint32_t m_Count;
	};
	
} // namespace Lucky
