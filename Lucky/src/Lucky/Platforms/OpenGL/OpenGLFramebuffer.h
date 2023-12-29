#pragma once

#include "Lucky/Renderer/Framebuffer.h"

namespace Lucky
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		FramebufferSpecification& GetSpecification() override { return m_Specification; }
		const FramebufferSpecification& GetSpecification() const override { return m_Specification; }

		void Bind() override;
		void Unbind() override;
		void Resize(uint32_t width, uint32_t height) override;

		uint32_t GetColorAttachmentRendererId() const override { return m_ColorAttachment; }

	private:
		void Invalidate();
		void Clean();

		FramebufferSpecification m_Specification;
		uint32_t m_FramebufferId = 0;
		uint32_t m_ColorAttachment = 0;
		uint32_t m_DepthAttachment = 0;
	};
}
