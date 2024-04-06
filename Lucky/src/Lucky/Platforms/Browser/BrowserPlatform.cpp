#include "LuckyPch.h"

#include "Lucky/Platforms/Platform.h"

namespace Lucky
{
	std::string Platform::OpenFile(const char* filter, const std::string& folder)
	{
		ImGui::OpenPopup("Open scene...");

		// Always center this window when appearing
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		static std::string item_current{}; // Here we store our selection data.

		if (ImGui::BeginPopupModal("Open scene...", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::BeginChild("##listbox", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing()));

			char* fileString = (char*)EM_ASM_INT({
				let dirPath = UTF8ToString($0);
				try {
					let scenes = FS.readdir(dirPath);
					scenes = scenes.join(";").replace(".;", "").replace("..;", "");
					return stringToNewUTF8(scenes);
				} catch (e) {
					return stringToNewUTF8("");
				}				
			}, (int)folder.c_str());

			std::vector<std::string> files;
			std::istringstream f(fileString);
			std::string s;    
			while (std::getline(f, s, ';')) {
				files.push_back(s);
			}
			free(fileString);

			for (const auto& fileName : files)
			{
				ImGui::PushID(fileName.c_str());
				const bool is_selected = (item_current == fileName);
				if (ImGui::Selectable(fileName.c_str(), is_selected))
					item_current = fileName;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
				ImGui::PopID();
			}

			ImGui::EndChild();

			if (ImGui::Button("OK", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
				std::string filePath = fmt::format("{0}/{1}", folder, item_current);
				item_current = {};
				return filePath;
			}

			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
				return std::string("##Cancel");
			}
			ImGui::EndPopup();
		}
		return std::string();
	}

	std::string Platform::SaveFile(const char* filter, const std::string& folder)
	{
		ImGui::OpenPopup("Save scene...");

		// Always center this window when appearing
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		static std::string item_current{}; // Here we store our selection data.

		if (ImGui::BeginPopupModal("Save scene...", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			static char filename[256] = "";
			ImGui::Text("File name");
			ImGui::InputText("##filename", filename, 256);

			auto cwd = std::filesystem::current_path();
			const auto& path = cwd.concat(folder);

			if (ImGui::Button("OK", ImVec2(90, 0)))
			{
				ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
				std::string filePath(filename);
				memset(filename, 0, sizeof(filename));
				return filePath.empty() ? std::string("##Cancel") : fmt::format("{0}/{1}{2}", path.c_str(), filePath, ".lucky");
			}

			ImGui::SameLine();
			if (ImGui::Button("Export", ImVec2(90, 0)))
			{
				ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
				std::string filePath(filename);
				memset(filename, 0, sizeof(filename));
				return filePath.empty() ? std::string("##Cancel") : fmt::format("--export={0}/{1}{2}", path.c_str(), filePath, ".lucky");
			}

			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(90, 0)))
			{
				ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
				memset(filename, 0, sizeof(filename));
				return std::string("##Cancel");
			}
			ImGui::EndPopup();
		}
		return std::string();
	}

	float Time::GetTime()
	{
		return (float)glfwGetTime();
	}
}
