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

		uint32_t GetColorAttachmentRendererId(uint32_t index = 0) const override { LK_CORE_ASSERT(index < m_ColorAttachments.size()); return m_ColorAttachments[index]; }
		void ClearAttachment(uint32_t attachmentIndex, int value) override;
		int ReadPixel(uint32_t attachmentIndex, int x, int y) const override;

	private:
		void Invalidate();
		void Clean();

		FramebufferSpecification m_Specification;
		uint32_t m_FramebufferId = 0;
	/*	uint32_t m_ColorAttachment = 0;
		uint32_t m_DepthAttachment = 0;*/

		//FramebufferTextureSpecification m_ColorAttachmentSpecs[10];
		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecs;
		FramebufferTextureSpecification m_DepthAttachmentSpec = FramebufferTextureFormat::None;

		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment;
	};
}
