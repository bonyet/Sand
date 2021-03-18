#include "AssetManagerPanel.h"
#include "Sand/Renderer/Texture.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <filesystem>
#include <array>

namespace Sand
{

	static std::filesystem::directory_entry s_SelectedDirectory;
	static std::array<Ref<Texture2D>, 7> s_FileIcons;

	static AssetManagerPanel::FileType GetAssetType(const std::filesystem::directory_entry& entry)
	{
		using FT = AssetManagerPanel::FileType;

		auto extension = entry.path().extension();
		std::string extString = extension.string();

		if (extString == "")
			return FT::Folder;
		if (extString == ".sscene")
			return FT::Scene;
		if (extString == ".txt")
			return FT::Text;
		if (extString == ".cs")
			return FT::Script;
		if (extString == ".ttf")
			return FT::Font;
		if (extString == ".glsl" || extString == ".shader")
			return FT::Shader;
		if (extString == ".png" || extString == ".jpg" 
			|| extString == ".PNG" || extString == ".JPG")
			return FT::Image;

		return FT::Unknown;
	}

	static std::string GetFileExtension(const std::filesystem::directory_entry& entry)
	{
		return entry.path().extension().string();
	}
	static std::string FileTypeToString(AssetManagerPanel::FileType type)
	{
		using FT = AssetManagerPanel::FileType;

		switch (type)
		{
		case FT::Folder:
			return "Folder";
		case FT::Scene:
			return "Scene";
		case FT::Script:
			return "Script";
		case FT::Text:
			return "Text";
		case FT::Image:
			return "Image";
		case FT::Shader:
			return "Shader";
		case FT::Font:
			return "Font";
		case FT::Unknown:
			return "Unknown";
		}

		return "Unknown";
	}

	AssetManagerPanel::AssetManagerPanel()
	{
		// LOAD ICONS
		{
			std::string iconFilepaths[7] = {
				"assets/textures/folder.png",
				"assets/textures/text.png",
				"assets/textures/script.png",
				"assets/textures/folder.png", // NEED SCENE
				"assets/textures/font.png",
				"assets/textures/folder.png", // NEED SHADER
				"assets/textures/image.png",
			};

			for (int i = 0; i < 7; i++)
			{
				s_FileIcons[i] = Texture2D::Create(iconFilepaths[i]);
			}
		}
	}

	void AssetManagerPanel::OnGuiRender()
	{
		if (!m_ShowWindow) {
			return;
		}

		ImGui::Begin("Asset Manager", &m_ShowWindow);

		ImGui::BeginChild("AssetManager_FolderView", { 200, ImGui::GetContentRegionAvail().y });
		{
			for (const auto& entry : std::filesystem::directory_iterator(mFilepath)) 
			{
				const auto entryName = entry.path().filename().string();
				
				std::string prefix = entry.is_directory() ? "Folder: " : "File: ";
				std::string text = prefix + entryName;

				if (ImGui::Selectable(text.c_str(), false)) {
					s_SelectedDirectory = entry;
				}

				ImGui::Separator();
			}
		}

		ImGui::EndChild();

		ImGui::SameLine();
		ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
		ImGui::SameLine();

		ImGui::BeginChild("AssetManager_FileView", ImGui::GetContentRegionAvail());
		{
			for (const auto& entry : std::filesystem::directory_iterator(s_SelectedDirectory))
			{

				std::string filename = entry.path().filename().string();
				filename.erase(filename.find(GetFileExtension(entry))); // Remove the extension from the name
				FileType type = GetAssetType(entry);

				Ref<Texture2D> icon = s_FileIcons[(int)type];
				std::string text = FileTypeToString(type) + ": " + filename;

				ImGui::Image(reinterpret_cast<ImTextureID>(icon->GetID()), { 20, 20 }, { 0, 1 }, { 1, 0 });
				ImGui::SameLine();
				ImGui::Text(text.c_str());
			}
		}
		ImGui::EndChild();

		ImGui::End();
	}

	void AssetManagerPanel::SetPath(const std::string& path)
	{
		mFilepath = path;
		s_SelectedDirectory = std::filesystem::directory_entry(path);
	}

}
