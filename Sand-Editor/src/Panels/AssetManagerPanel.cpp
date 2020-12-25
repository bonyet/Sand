#include "AssetManagerPanel.h"

#include <imgui/imgui.h>
#include <glm/glm.hpp>

namespace Sand
{

	void AssetManagerPanel::OnGuiRender()
	{
		ImGui::Begin("Asset Manager");

		ImGui::Columns(2);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
		// ICONS WINDOW
		{
			ImGui::SetCursorPosX(0.0f);
			ImVec2 iconsWindowSize = { ImGui::GetContentRegionMax().x, ImGui::GetContentRegionAvail().y };
			ImGui::BeginChildFrame(69420, iconsWindowSize);

			ImGui::Text("This is the icons window! Huzzah!");

			ImGui::EndChild();
		}
		ImGui::NextColumn();
		ImGui::SameLine();

		// FOLDERS VIEW WINDOW
		{
			ImVec2 foldersWindowSize = { ImGui::GetContentRegionMax().x, ImGui::GetContentRegionAvail().y };

			ImGui::BeginChildFrame(69421, foldersWindowSize, false);

			ImGui::Text("Folders! Yay!");

			ImGui::EndChild();
		}
		ImGui::PopStyleVar();
		ImGui::Columns(1);

		ImGui::End();

	}

}