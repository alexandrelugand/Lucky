#include "LuckyPch.h"

#ifdef PLATFORM_WINDOWS
#include "Lucky/Platforms/Platform.h"
#include "Lucky/Application/Application.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <commdlg.h>

namespace Lucky
{
	std::string Platform::OpenFile(const char* filter, const std::string& folder)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
		ofn.lpstrFile = szFile;
		auto cwd = std::filesystem::current_path();
		auto path = fmt::format("{0}\\{1}", ToNarrow(cwd.c_str()), folder);
		std::ranges::replace(path, '/', '\\');
		ofn.lpstrInitialDir = path.c_str();
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetOpenFileNameA(&ofn) == TRUE)
			return ofn.lpstrFile;

		return std::string("##Cancel");
	}

	std::string Platform::SaveFile(const char* filter, const std::string& folder)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
		ofn.lpstrFile = szFile;
		auto cwd = std::filesystem::current_path();
		auto path = fmt::format("{0}\\{1}", ToNarrow(cwd.c_str()), folder);
		std::ranges::replace(path, '/', '\\');
		ofn.lpstrInitialDir = path.c_str();
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetSaveFileNameA(&ofn) == TRUE)
		{
			if (ofn.nFileExtension == 0)
			{
				// add default extension, no extension was selected/entered by user
				return ofn.lpstrFile + std::string(".lucky");
			}
			// there is an extension, save as entered.
			return ofn.lpstrFile;
		}

		return std::string("##Cancel");
	}

	float Time::GetTime()
	{
		return (float)glfwGetTime();
	}
}

#endif
