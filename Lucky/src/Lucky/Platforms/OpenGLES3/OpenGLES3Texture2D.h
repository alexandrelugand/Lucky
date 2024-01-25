#pragma once

#include "Lucky/Renderer/Texture.h"

namespace Lucky
{
	class OpenGLES3Texture2D : public Texture2D
	{
	public:
		OpenGLES3Texture2D(uint32_t width, uint32_t height);
		OpenGLES3Texture2D(const std::string& path);
		virtual ~OpenGLES3Texture2D();

		uint32_t GetRendererId() const override { return m_TextureId; }

		uint32_t GetWidth() const override { return m_Width; }
		uint32_t GetHeight() const override { return m_Height; }

		const std::string GetPath() const override { return m_Path; }

		void Bind(uint32_t slot = 0) override;
		void SetData(void* data, uint32_t size) override;

		bool IsLoaded() const override { return m_IsLoaded; }

		bool operator==(const Texture& other) const override { return m_TextureId == ((OpenGLES3Texture2D&)other).m_TextureId; }

	private:
		uint32_t m_TextureId;
		uint32_t m_Width, m_Height;
		uint32_t m_InternalFormat, m_DataFormat;
		std::string m_Path = "";
		bool m_IsLoaded = false;
	};
}
