#include "LuckyPch.h"
#include "OpenGLTexture2D.h"

namespace Lucky
{
	OpenGLTexture2D::OpenGLTexture2D(const std::string &path)
	{
		int width, height;
		int channels;
		stbi_set_flip_vertically_on_load(1);
		auto data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		LK_CORE_ASSERT(data, "Failed to load image!");

		m_Width = width;
		m_Height = height;
		GLenum internalFormat = 0, dataFormat = 0;
		if(channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
			m_Alpha = true;
		}
		else if(channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
			m_Alpha = false;
		}

		LK_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

		LK_CORE_TRACE("Image path: {0}", path);
		LK_CORE_TRACE("Image {0}x{1} pixels", m_Width, m_Height);

		glGenTextures(1, &m_TextureId);
		glBindTexture(GL_TEXTURE_2D, m_TextureId);
		glTexStorage2D(GL_TEXTURE_2D, 1, internalFormat, m_Width, m_Height);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_TextureId);
	}

	void OpenGLTexture2D::Bind(uint32_t slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		if(m_Alpha)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		glBindTexture(GL_TEXTURE_2D, m_TextureId);
	}

} // namespace Lucky
