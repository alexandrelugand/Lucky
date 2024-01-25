#pragma once
#include <filesystem>

#include "Lucky/Core/Base.h"
#include "Lucky/Renderer/Texture.h"

namespace Lucky
{
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		void OnImGuiRenderer();

	private:
		std::filesystem::path m_CurrentDirectory;
		Ref<Texture2D>m_DirectoryIcon;
		Ref<Texture2D> m_FileIcon;
	};
}