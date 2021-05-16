// Created by David Gallardo on 11/06/16.

#include <sandpch.h>

#include <imgui.h>
#include "ImGuiColorGradient.h"
#include "imgui_internal.h"

static constexpr float GRADIENT_BAR_WIDGET_HEIGHT = 25;
static constexpr float GRADIENT_BAR_EDITOR_HEIGHT = 40;
static constexpr float GRADIENT_MARK_DELETE_DIFFY = 40;

using namespace Sand;

namespace ImGui
{
	static void DrawGradientBar(ColorGradient* gradient, const ImVec2& barPos, float maxWidth, float height)
	{
		ImVec4 colorA = { 1.0f, 1.0f, 1.0f, 1.0f };
		ImVec4 colorB = { 1.0f, 1.0f, 1.0f, 1.0f };

		float prevX = barPos.x;
		float barBottom = barPos.y + height;
		GradientMark* prevMark = nullptr;
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		drawList->AddRectFilled(ImVec2(barPos.x - 2, barPos.y - 2),
			ImVec2(barPos.x + maxWidth + 2, barBottom + 2),
			IM_COL32(100, 100, 100, 255));

		// If there are no marks, rect = white
		if (gradient->GetMarks().size() == 0)
		{
			drawList->AddRectFilled(ImVec2(barPos.x, barPos.y),
				ImVec2(barPos.x + maxWidth, barBottom),
				IM_COL32(255, 255, 255, 255));
		}

		ImU32 colorAU32 = 0, colorBU32 = 0;

		for (auto& gradientMarkRef : gradient->GetMarks())
		{
			GradientMark* mark = &gradientMarkRef;

			float from = prevX;
			float to = prevX = barPos.x + mark->position * maxWidth;

			if (prevMark == nullptr)
			{
				colorA.x = mark->color[0];
				colorA.y = mark->color[1];
				colorA.z = mark->color[2];
				colorA.w = mark->color[3];
			}
			else
			{
				colorA.x = prevMark->color[0];
				colorA.y = prevMark->color[1];
				colorA.z = prevMark->color[2];
				colorA.w = prevMark->color[3];
			}

			colorB.x = mark->color[0];
			colorB.y = mark->color[1];
			colorB.z = mark->color[2];
			colorB.w = mark->color[3];

			colorAU32 = ImGui::ColorConvertFloat4ToU32(colorA);
			colorBU32 = ImGui::ColorConvertFloat4ToU32(colorB);

			if (mark->position > 0.0)
			{
				drawList->AddRectFilledMultiColor(ImVec2(from, barPos.y),
					{ to, barBottom }, colorAU32, colorBU32, colorBU32, colorAU32);
			}

			prevMark = mark;
		}

		if (prevMark && prevMark->position < 1.0)
		{
			drawList->AddRectFilledMultiColor(ImVec2(prevX, barPos.y),
				{ barPos.x + maxWidth, barBottom },
				colorBU32, colorBU32, colorBU32, colorBU32);
		}

		ImGui::SetCursorScreenPos(ImVec2(barPos.x, barPos.y + height + 10.0f));
	}

	static void DrawGradientMarks(ColorGradient* gradient, GradientMark*& draggingMark, GradientMark*& selectedMark, const ImVec2& barPos, float maxWidth, float height)
	{
		ImVec4 colorA = { 1.0f, 1.0f, 1.0f, 1.0f };
		ImVec4 colorB = { 1.0f, 1.0f, 1.0f, 1.0f };

		float barBottom = barPos.y + height;
		GradientMark* prevMark = nullptr;
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		ImU32 colorAU32 = 0, colorBU32 = 0;

		bool deleteSelectedMark = false;
		GradientMark* mark = nullptr;

		for (auto& gradientMarkRef : gradient->GetMarks())
		{
			mark = &gradientMarkRef;

			if (!selectedMark)
				selectedMark = mark;

			float to = barPos.x + mark->position * maxWidth;

			if (prevMark == nullptr)
			{
				colorA.x = mark->color[0];
				colorA.y = mark->color[1];
				colorA.z = mark->color[2];
				colorA.w = mark->color[3];
			}
			else
			{
				colorA.x = prevMark->color[0];
				colorA.y = prevMark->color[1];
				colorA.z = prevMark->color[2];
				colorA.w = prevMark->color[3];
			}

			colorB.x = mark->color[0];
			colorB.y = mark->color[1];
			colorB.z = mark->color[2];
			colorB.w = mark->color[3];

			colorAU32 = ImGui::ColorConvertFloat4ToU32(colorA);
			colorBU32 = ImGui::ColorConvertFloat4ToU32(colorB);

			drawList->AddTriangleFilled(ImVec2(to, barPos.y + (height - 6)),
				ImVec2(to - 6, barBottom),
				ImVec2(to + 6, barBottom), IM_COL32(100, 100, 100, 255));

			drawList->AddRectFilled(ImVec2(to - 6, barBottom),
				ImVec2(to + 6, barPos.y + (height + 12)),
				IM_COL32(100, 100, 100, 255), 1.0f, 1.0f);

			drawList->AddRectFilled(ImVec2(to - 5, barPos.y + (height + 1)),
				ImVec2(to + 5, barPos.y + (height + 11)),
				IM_COL32(0, 0, 0, 255), 1.0f, 1.0f);

			if (selectedMark == mark)
			{
				drawList->AddTriangleFilled(ImVec2(to, barPos.y + (height - 3)),
					ImVec2(to - 4, barBottom + 1),
					ImVec2(to + 4, barBottom + 1), IM_COL32(0, 255, 0, 255));

				drawList->AddRect(ImVec2(to - 5, barPos.y + (height + 1)),
					ImVec2(to + 5, barPos.y + (height + 11)),
					IM_COL32(0, 255, 0, 255), 1.0f, 1.0f);
			}

			drawList->AddRectFilledMultiColor(ImVec2(to - 3, barPos.y + (height + 3)),
				ImVec2(to + 3, barPos.y + (height + 9)),
				colorBU32, colorBU32, colorBU32, colorBU32);

			ImGui::SetCursorScreenPos(ImVec2(to - 6, barBottom));
			ImGui::InvisibleButton("Mark", ImVec2(12, 12));

			// Selecting marks
			if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
			{
				SAND_CORE_TRACE("Select");
				selectedMark = mark;
				draggingMark = mark;
			}

			// Deleting marks
			if (ImGui::IsKeyPressedMap(ImGuiKey_Delete, false))
			{
				// Don't let them remove all the marks
				if (gradient->GetMarks().size() > 1 && *selectedMark != gradient->GetMarks().front())
				{
					deleteSelectedMark = true;
				}
			}

			prevMark = mark;
		}

		if (deleteSelectedMark)
		{
			gradient->RemoveMark(*selectedMark);
			selectedMark = nullptr;
		}

		ImGui::SetCursorScreenPos(ImVec2(barPos.x, barPos.y + height + 20.0f));
	}

	bool GradientButton(ColorGradient* gradient)
	{
		if (!gradient) 
			return false;

		ImVec2 widgetPos = ImGui::GetCursorScreenPos();

		float maxWidth = ImMax(250.0f, ImGui::GetContentRegionAvailWidth() - 100.0f);
		bool clicked = ImGui::InvisibleButton("Gradient_Bar", ImVec2(maxWidth, GRADIENT_BAR_WIDGET_HEIGHT));

		DrawGradientBar(gradient, widgetPos, maxWidth, GRADIENT_BAR_WIDGET_HEIGHT);

		return clicked;
	}

	bool GradientEditor(ColorGradient* gradient, GradientMark*& draggingMark, GradientMark*& selectedMark)
	{
		if (!gradient)
			return false;

		bool modified = false;

		ImVec2 barPos = ImGui::GetCursorScreenPos();
		barPos.x += 10;
		float maxWidth = ImGui::GetContentRegionAvailWidth() - 20;
		float barBottom = barPos.y + GRADIENT_BAR_EDITOR_HEIGHT;

		ImGui::InvisibleButton("GradientEditorBar", ImVec2(maxWidth, GRADIENT_BAR_EDITOR_HEIGHT));

		if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
		{
			float pos = (ImGui::GetIO().MousePos.x - barPos.x) / maxWidth;

			float newMarkCol[4];
			gradient->Evaluate(pos, newMarkCol);

			gradient->AddMark(pos, newMarkCol[0], newMarkCol[1], newMarkCol[2], newMarkCol[3]);
		}

		DrawGradientBar(gradient, barPos, maxWidth, GRADIENT_BAR_EDITOR_HEIGHT);
		DrawGradientMarks(gradient, draggingMark, selectedMark, barPos, maxWidth, GRADIENT_BAR_EDITOR_HEIGHT);

		if (!ImGui::IsMouseDown(0) && draggingMark)
		{
			draggingMark = nullptr;
		}

		if (ImGui::IsMouseDragging(0) && draggingMark)
		{
			float increment = ImGui::GetIO().MouseDelta.x / maxWidth;
			bool insideZone = (ImGui::GetIO().MousePos.x > barPos.x) &&
				(ImGui::GetIO().MousePos.x < barPos.x + maxWidth);

			if (increment != 0.0f && insideZone)
			{
				draggingMark->position += increment;
				draggingMark->position = ImClamp(draggingMark->position, 0.0f, 1.0f);
				gradient->RefreshCache();
				modified = true;
			}

			float diffY = ImGui::GetIO().MousePos.y - barBottom;

			if (diffY >= GRADIENT_MARK_DELETE_DIFFY)
			{
				gradient->RemoveMark(*draggingMark);
				draggingMark = selectedMark = nullptr;
				modified = true;
			}
		}

		if (!selectedMark && gradient->GetMarks().size() > 0)
		{
			selectedMark = &gradient->GetMarks().front();
		}

		if (selectedMark)
		{
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 50.0f);
			ImGui::SetNextItemWidth(300.0f);

			bool colorModified = ImGui::ColorPicker3("##ColorPick", selectedMark->color, ImGuiColorEditFlags_NoBorder | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview);
			
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 50.0f);
			
			ImGui::NewLine();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 50.0f);

			ImGui::SliderFloat4("##Color", selectedMark->color, 0.0f, 1.0f, "%.2f");

			if (selectedMark && colorModified)
			{
				modified = true;
				gradient->RefreshCache();
			}
		}

		return modified;
	}
};