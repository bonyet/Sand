#include "ConsolePanel.h"

#include "Sand/Debug/Debug.h"
#include "Sand/Core/Time.h"
#include "Sand/Core/Log.h"

#include <glm/glm.hpp>

#include <imgui/imgui.h>
#include <imgui\imgui_internal.h>

namespace Sand
{
	std::vector<std::tuple<std::string, glm::vec3, std::string>> Debug::m_DebugMessages{};

	static std::array<Ref<Texture2D>, 3> s_IconTextures;

	ConsolePanel::ConsolePanel()
	{
		s_IconTextures = {
			Texture2D::Create("assets/textures/information.png"),
			Texture2D::Create("assets/textures/warning.png"),
			Texture2D::Create("assets/textures/error.png"),
		};
	}

	void ConsolePanel::OnGuiRender()
	{
		if (!m_ShowWindow) {
			if (Debug::GetMessages().size() != 0)
				Debug::GetMessages().clear();
			return;
		}

		ImGui::Begin("Console", &m_ShowWindow);

		// TOOLS SECTION
		static int visibilityType = 0;
		{
			// CLEAR
			if (ImGui::Button("Clear", { 50, 25 })) {
				Debug::GetMessages().clear();
			}

			ImGui::SameLine();
			ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
			ImGui::SameLine();

			// VISIBILITY
			// Determine width of combo box based on the selected visibility setting
			ImGui::Text("Visibility");
			ImGui::SameLine();
			switch (visibilityType) {
				case 0:
					ImGui::SetNextItemWidth(60);
					break;
				case 1:
					ImGui::SetNextItemWidth(60);
					break;
				case 2:
					ImGui::SetNextItemWidth(110);
					break;
				case 3:
					ImGui::SetNextItemWidth(80);
					break;
			}
			
			const char* visibilityTypes[] = { "All", "Info", "Warnings", "Errors" };
			const char* currentVisibilityType = visibilityTypes[visibilityType];
			if (ImGui::BeginCombo("##Visibility", currentVisibilityType))
			{
				for (int i = 0; i < 4; i++)
				{
					bool isSelected = currentVisibilityType == visibilityTypes[i];
					if (ImGui::Selectable(visibilityTypes[i], isSelected))
					{
						currentVisibilityType = visibilityTypes[i];
						visibilityType = i;
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			};
		}

		ImGui::Separator();
		ImGui::Spacing();

		// MESSAGES SECTION
		{
			float y = ImGui::GetCursorPosY();
			ImGui::SetCursorPosY(y - 4);
			ImGui::BeginChild("Console_Messages", { ImGui::GetWindowContentRegionMax().x, ImGui::GetWindowContentRegionMax().y - y });
			bool shouldScroll = ImGui::GetScrollY() == ImGui::GetScrollMaxY();

			constexpr glm::vec3 colors[3] = { Debug::InfoMessageColor, Debug::WarningMessageColor, Debug::ErrorMessageColor };

			// display messages
			for (int i = 0; i < Debug::GetMessages().size(); i++)
			{
				auto& element = Debug::GetMessages()[i];
				auto color = std::get<1>(element);

				if (visibilityType != 0 && color != colors[visibilityType - 1]) {
					continue;
				}

				// TODO: icons
				{
					Ref<Texture2D> iconTexture;
					// *barfing*
					if (color == Debug::InfoMessageColor) {
						iconTexture = s_IconTextures[0];
					}
					else if (color == Debug::WarningMessageColor) {
						iconTexture = s_IconTextures[1];
					}
					else if (color == Debug::ErrorMessageColor) {
						iconTexture = s_IconTextures[2];
					}
					//*stops barfing*

					ImGui::Image((ImTextureID)iconTexture->GetID(), { 20, 20 }, { 0,1 }, { 1, 0 });
					ImGui::SameLine();
				}

				ImGui::TextColored({ 1.0f, 1.0f, 1.0f, 0.5f }, std::get<0>(element).c_str());
				ImGui::SameLine();
				float cursorX = ImGui::GetCursorPosX();
				ImGui::TextColored({ color.r, color.b, color.g, 1.0f }, std::get<2>(element).c_str());
				ImGui::SameLine(cursorX, 0);
				if (ImGui::InvisibleButton((std::string("##Button") + std::to_string(i)).c_str(), ImGui::GetItemRectSize())) {
					// TODO: open to where the Debug::Print was called
				}
				ImGui::Separator();

				if (shouldScroll) {
					ImGui::SetScrollHereY();
				}
			}
			ImGui::EndChild();
		}

		ImGui::End();
	}

	void ConsolePanel::sink_it_(const spdlog::details::log_msg& msg)
	{
		switch (msg.level) {
			case spdlog::level::trace:
			case spdlog::level::info:
			{
				Debug::PrintInfo(std::string(msg.payload));
				return;
			}
			case spdlog::level::warn:
			{
				Debug::PrintWarning(std::string(msg.payload));
				return;
			case spdlog::level::err:
			case spdlog::level::critical:
			{
				Debug::PrintError(std::string(msg.payload));
				return;
			}
		}
		}
	}

	void ConsolePanel::flush_()
	{
	}

}