#pragma once

namespace ImGui
{
	void TextTooltip(const char* text, const char* tooltip, float delay = 0.75f);
	void Tooltip(const char* text, float delay = 0.75f);
	void TextScaled(float scale, const char* text, ...);

	bool ToggleButton(const char* str_id, bool* v);
}