#include "LuckyPch.h"
#include "OpenGLES3Texture2D.h"

namespace Lucky
{
	OpenGLES3Texture2D::OpenGLES3Texture2D(uint32_t width, uint32_t height)
	:	m_Width(width), m_Height(height)
	{
		LK_PROFILE_FUNCTION();
		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glGenTextures(1, &m_TextureId);
		glBindTexture(GL_TEXTURE_2D, m_TextureId);
		glTexStorage2D(GL_TEXTURE_2D, 1, m_InternalFormat, m_Width, m_Height);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	OpenGLES3Texture2D::OpenGLES3Texture2D(const std::string &path)
		: m_Path(path)
	{
		LK_PROFILE_FUNCTION();
		int width, height;
		int channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			LK_PROFILE_SCOPE("OpenGLES3Texture2D::OpenGLES3Texture2D - stbi_load");
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}

		if(data)
		{
			m_IsLoaded = true;

			m_Width = width;
			m_Height = height;
			if (channels == 4)
			{
				m_InternalFormat = GL_RGBA8;
				m_DataFormat = GL_RGBA;
			}
			else if (channels == 3)
			{
				m_InternalFormat = GL_RGB8;
				m_DataFormat = GL_RGB;
			}

			LK_CORE_ASSERT(m_InternalFormat & m_DataFormat, "Format not supported!");

			LK_CORE_TRACE("Image path: {0}", path);
			LK_CORE_TRACE("Image {0}x{1} pixels", m_Width, m_Height);

			glGenTextures(1, &m_TextureId);
			glBindTexture(GL_TEXTURE_2D, m_TextureId);
			glTexStorage2D(GL_TEXTURE_2D, 1, m_InternalFormat, m_Width, m_Height);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			stbi_image_free(data);
		}
	}

	OpenGLES3Texture2D::~OpenGLES3Texture2D()
	{
		LK_PROFILE_FUNCTION();
		glDeleteTextures(1, &m_TextureId);
	}

	void OpenGLES3Texture2D::Bind(uint32_t slot)
	{
		LK_PROFILE_FUNCTION();
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_TextureId);
	}

	void OpenGLES3Texture2D::SetData(void *data, uint32_t size)
	{
		LK_PROFILE_FUNCTION();
		LK_CORE_ASSERT(size == m_Width * m_Height * (m_DataFormat == GL_RGBA ? 4 : 3), "Data must be entire texture!")
		glBindTexture(GL_TEXTURE_2D, m_TextureId);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

}
