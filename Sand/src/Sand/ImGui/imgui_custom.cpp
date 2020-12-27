#include "sandpch.h"
#include "imgui_custom.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace ImGui
{
	void TextTooltip(const char* text, const char* tooltip)
	{
		float cursorX = ImGui::GetCursorPosX();
		ImGui::Text(text);
		ImGui::SameLine(cursorX, 0);
		ImGui::InvisibleButton("##button", ImGui::GetItemRectSize());

		if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.75f)
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(tooltip);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}
	void Tooltip(const char* text)
	{
		if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.75f)
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(text);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}
}