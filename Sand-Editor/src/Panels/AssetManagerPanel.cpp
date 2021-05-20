#include "AssetManagerPanel.h"
#include "Sand/Renderer/Texture.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <array>

namespace Sand
{

	struct FileIcon
	{
		Ref<Texture2D> Texture;
		std::string TexturePath;
		ImVec2 PreferredSize = { 25, 25 };
	};

	static std::array<FileIcon, 7> s_FileIcons;

	AssetManagerPanel::AssetManagerPanel()
	{
		// LOAD ICONS
		{
			std::pair<std::string, ImVec2> iconPathsAndSizes[7] = {
				{"assets/textures/folder.png", { 25, 25 }},
				{"assets/textures/text.png",   { 25, 25 }},
				{"assets/textures/script.png", { 35, 35 }},
				{"assets/textures/folder.png", { 25, 25 }}, // NEED SCENE
				{"assets/textures/font.png",   { 15, 15 }},
				{"assets/textures/folder.png", { 20, 20 }},  // NEED SHADER
				{"assets/textures/image.png",  { 25, 25 }},
			};

			for (int i = 0; i < 7; i++)
			{
				auto element = iconPathsAndSizes[i];
				s_FileIcons[i] = { Texture2D::Create(element.first), element.first, element.second };
			}
		}
	}

	static AssetFileType GetFileType(const std::filesystem::directory_entry& entry)
	{
		// Is it a folder?
		if (entry.is_directory())
			return AssetFileType::Folder;

		std::string string = entry.path().extension().string();

		// Is it an image?
		{
			constexpr size_t numImageFileExtensions = 2;
			const char* imageFileTypesString[numImageFileExtensions] = {
				".png", ".jpg"
			};
			for (int i = 0; i < numImageFileExtensions; i++)
			{
				if (strcmp(string.c_str(), imageFileTypesString[i]) == 0)
					return AssetFileType::Image;
			}
		}

		// Handle other file extensions
		constexpr size_t numFileExtensions = 6;
		const char* assetFileTypesString[numFileExtensions] = {
			".txt", ".cs", ".sscene", ".ttf", ".glsl", 
		};

		for (int i = 1; i < numFileExtensions; i++)
		{
			if (strcmp(string.c_str(), assetFileTypesString[i - 1]) == 0) {
				return (AssetFileType)i;
			}
		}

		return AssetFileType::Unknown;
	}

	static FileIcon& GetFileIcon(const std::filesystem::directory_entry& entry)
	{
		AssetFileType type = GetFileType(entry);

		return s_FileIcons[(int)type];
	}

	void AssetManagerPanel::OnGuiRender()
	{
		if (!m_ShowWindow) {
			return;
		}

		ImGui::Begin("Asset Manager", &m_ShowWindow);

		ImGui::BeginChild("AssetManager_FolderView", { 200, ImGui::GetContentRegionAvail().y });
		{
			for (const auto& entry : std::filesystem::directory_iterator(m_Filepath)) 
			{
				const auto entryName = entry.path().filename().string();
				
				std::string prefix = entry.is_directory() ? "Folder: " : "File: ";
				std::string text = prefix + entryName;

				// Select folder
				if (ImGui::Selectable(text.c_str(), false)) 
				{
					m_CurrentDirectory = entry;
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
			for (const auto& entry : std::filesystem::directory_iterator(m_CurrentDirectory))
			{
				// display file preview
				auto& icon = GetFileIcon(entry);
				if (icon.Texture)
				{
					ImGui::Image(reinterpret_cast<ImTextureID>(icon.Texture->GetID()), icon.PreferredSize, { 0, 1 }, { 1, 0 });
					ImGui::SameLine();
					ImGui::Text("%s", entry.path().filename().string().c_str());
				} else 
				{
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.8f, 0.2f, 0.3f, 1.0f });
					ImGui::Text("Unknown file type");
					ImGui::PopStyleColor();
				}
			}
		}
		ImGui::EndChild();

		ImGui::End();
	}

	void AssetManagerPanel::SetPath(const std::string& path)
	{
		m_Filepath = path;
		m_CurrentDirectory = { path };
	}

}
