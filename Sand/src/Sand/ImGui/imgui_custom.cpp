#include "sandpch.h"
#include "imgui_custom.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace ImGui
{
	void TextTooltip(const char* text, const char* tooltip, float delay)
	{
		float cursorX = ImGui::GetCursorPosX();
		ImGui::Text(text);
		ImGui::SameLine(cursorX, 0);
		ImGui::InvisibleButton("##button", ImGui::GetItemRectSize());

		if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > delay)
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(tooltip);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

	void Tooltip(const char* text, float delay)
	{
		if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > delay)
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(text);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

	bool ToggleButton(const char* str_id, bool* v)
	{
		ImGui::Button(str_id, { 40, 25 });
		if (ImGui::IsItemClicked()) {
			*v = !*v;
			return true;
		}

		return false;
	}
}