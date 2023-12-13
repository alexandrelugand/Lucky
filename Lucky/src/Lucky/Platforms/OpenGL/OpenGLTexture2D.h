#pragma once

#include "Lucky/Core/Renderer/Texture.h"

namespace Lucky
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		inline uint32_t GetWidth() const override { return m_Width; }
		inline uint32_t GetHeight() const override { return m_Height; }

		void Bind(uint32_t slot = 0) override;

	private:
		uint32_t m_TextureId;
		uint32_t m_Width, m_Height;
		bool m_Alpha;
	};
} // namespace Lucky
