#pragma once 

#include "Lucky/Renderer/IndexBuffer.h"

namespace Lucky
{
	class OpenGLES3IndexBuffer : public IndexBuffer
	{
	public:
		OpenGLES3IndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLES3IndexBuffer();

		void Bind() const override;
		void Unbind() const override;
		inline uint32_t GetCount() const override { return m_Count; }

	private:
		uint32_t m_IndexBufferId;
		uint32_t m_Count;
	};
	
}
