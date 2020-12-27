#include "PropertiesPanel.h"
#include "Sand/Scene/Components.h"
#include "Sand/ImGui/imgui_custom.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#define SAND_LEFT_LABEL(func, label, code) ImGui::TextUnformatted(label); ImGui::NextColumn(); ImGui::SetNextItemWidth(-1); if(func) { code } ImGui::NextColumn();
#define SAND_LEFT_LABEL_TOOLTIP(func, label, tooltip, code) ImGui::TextTooltip(label, tooltip); ImGui::NextColumn(); ImGui::SetNextItemWidth(-1); if(func) { code } ImGui::NextColumn();

namespace Sand
{
	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
	{
		constexpr ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		if (!entity.HasComponent<T>())
			return;

		auto& component = entity.GetComponent<T>();
		ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 2, 2 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImGui::Spacing();
		ImGui::Separator();
		bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
		ImGui::PopStyleVar();
		ImGui::Tooltip("Right click for more actions");

		if (ImGui::IsItemClicked(1))
			ImGui::OpenPopup("ComponentSettings");

		bool removeComponent = false;
		if (ImGui::BeginPopup("ComponentSettings"))
		{
			if (ImGui::MenuItem("Remove component"))
				removeComponent = true;

			ImGui::EndPopup();
		}

		if (open)
		{
			ImGui::Columns(2);
			uiFunction(component);
			ImGui::Columns(1);
			ImGui::TreePop();
		}

		if (removeComponent)
			entity.RemoveComponent<T>();

		ImGui::Spacing();
	}

	void PropertiesPanel::OnGuiRender()
	{
		if (!m_ShowWindow)
			return;

		ImGui::Begin("Properties", &m_ShowWindow);

		if (m_SelectionContext)
		{
			DrawComponents(m_SelectionContext);
		}

		ImGui::End();
	}

	static void DrawVectorControl(const std::string& label, glm::vec3& vector, float resetValue = 0.0f, float columnWidth = 100.0f, 
		const std::string& tooltip = std::string())
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[1];

		ImGui::PushID(label.c_str());

		if (tooltip != std::string()) {
			SAND_LEFT_LABEL_TOOLTIP(ImGui::DragFloat3("##Drag", glm::value_ptr(vector), 0.1f, 0.0f, 0.0f, "%.2f"), label.c_str(), tooltip.c_str(), );
		}
		else {
			SAND_LEFT_LABEL(ImGui::DragFloat3("##Drag", glm::value_ptr(vector), 0.1f, 0.0f, 0.0f, "%.2f"), label.c_str(), );
		}
		
		//ImGui::Text(label.c_str()); 
		//
		//ImGui::NextColumn();
		//ImGui::SameLine();
		//ImGui::NextColumn();
		//
		//ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		//ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
		//
		//float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		//ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
		//
		//ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		//ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		//ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		//ImGui::PushFont(boldFont);
		//
		//ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
		//if (ImGui::Button("X", buttonSize))
		//	vector.x = resetValue;
		//ImGui::PopFont();
		//ImGui::PopStyleColor(3);
		//ImGui::PopStyleVar();
		//ImGui::SameLine();
		//ImGui::DragFloat("##X", &vector.x, 0.1f, 0.0f, 0.0f, "%.2f");
		//ImGui::PopItemWidth();
		//ImGui::SameLine();
		//
		//ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.3f, 1.0f });
		//ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		//ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		//
		//ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
		//ImGui::PushFont(boldFont);
		//if (ImGui::Button("Y", buttonSize))
		//	vector.y = resetValue;
		//ImGui::PopFont();
		//ImGui::PopStyleColor(3);
		//ImGui::PopStyleVar();
		//ImGui::SameLine();
		//ImGui::DragFloat("##Y", &vector.y, 0.1f, 0.0f, 0.0f, "%.2f");
		//ImGui::PopItemWidth();
		//ImGui::SameLine();
		//
		//ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		//ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		//ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		//
		//ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
		//ImGui::PushFont(boldFont);
		//if (ImGui::Button("Z", buttonSize))
		//	vector.z = resetValue;
		//ImGui::PopFont();
		//ImGui::PopStyleColor(3);
		//ImGui::PopStyleVar();
		//ImGui::SameLine();
		//ImGui::DragFloat("##Z", &vector.z, 0.1f, 0.0f, 0.0f, "%.2f");
		//ImGui::PopItemWidth();
		//
		//ImGui::PopStyleVar();

		ImGui::PopID();
	}

	template<typename T>
	static void DrawComponentMenuItem(const std::string& title, Entity entity)
	{
		if (ImGui::MenuItem(title.c_str()))
		{
			if (!entity.HasComponent<T>()) {
				entity.AddComponent<T>();
			}
			else
			{
				SAND_CORE_WARN("{0} component already present on '{1}'", title, entity.GetComponent<TagComponent>().Name);
			}

			ImGui::CloseCurrentPopup();
		}
	}
	
	void PropertiesPanel::DrawComponentsMenu(Entity entity)
	{
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			DrawComponentMenuItem<CameraComponent>("Camera", entity);
			DrawComponentMenuItem<TransformComponent>("Transform", entity);
			DrawComponentMenuItem<SpriteRendererComponent>("Sprite Renderer", entity);

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();
	}

	void PropertiesPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Name;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		ImGui::SameLine();

		DrawComponentsMenu(entity);

		DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
		{
			DrawVectorControl("Position", component.Position, 0.0f, 100.0f, "The world space position of this entity, in meters");

			// Rotation in euler angles for UI, radians internally so we convert
			glm::vec3 eulerAngles = glm::degrees(component.Rotation);
			DrawVectorControl("Rotation", eulerAngles, 0.0f, 100.0f, "The rotation of this entity in euler angles");
			component.Rotation = glm::radians(eulerAngles);

			DrawVectorControl("Scale", component.Scale, 1.0f, 100.0f, "The scale of this entity in meters");
		});

		DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
		{
			auto& camera = component.Camera;

			SAND_LEFT_LABEL_TOOLTIP(ImGui::Checkbox("##Primary", &component.Primary), "Primary", "Primary cameras render the scene", );

			const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
			const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];
			SAND_LEFT_LABEL_TOOLTIP(ImGui::BeginCombo("##Projection", currentProjectionTypeString), "Projection", "Perspective cameras are most commonly used in 3D environments, and Orthographic for 2D",
			{
				for (int i = 0; i < 2; i++)
				{
					bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
					if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
					{
						currentProjectionTypeString = projectionTypeStrings[i];
						camera.SetProjectionType((SceneCamera::ProjectionType)i);
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			});

			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
			{
				float perspectiveVerticalFov = glm::degrees(camera.GetPerspectiveFOV());
				SAND_LEFT_LABEL_TOOLTIP(ImGui::DragFloat("##Vertical FOV", &perspectiveVerticalFov), "Vertical FOV", "Higher levels will cause more distortion",
					camera.SetPerspectiveFOV(glm::radians(perspectiveVerticalFov));
				);
			
				float perspectiveNear = camera.GetPerspectiveNearClip();
				SAND_LEFT_LABEL_TOOLTIP(ImGui::DragFloat("##Near", &perspectiveNear), "Near", "Objects closer to the camera than this value will be clipped",
					camera.SetPerspectiveNearClip(perspectiveNear);
				);
			
				float perspectiveFar = camera.GetPerspectiveFarClip();
				SAND_LEFT_LABEL_TOOLTIP(ImGui::DragFloat("##Far", &perspectiveFar), "Far", "Objects further from the camera than this value will be clipped",
					camera.SetPerspectiveFarClip(perspectiveFar);
				);
			}
			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
			{
				float orthoSize = camera.GetOrthographicSize();
				SAND_LEFT_LABEL(ImGui::DragFloat("##Size", &orthoSize), "Size",
					camera.SetOrthographicSize(orthoSize);
				);
			
				float orthoNear = camera.GetOrthographicNearClip();
				SAND_LEFT_LABEL_TOOLTIP(ImGui::DragFloat("##Near", &orthoNear), "Near", "Objects closer to the camera than this value will be clipped",
					camera.SetOrthographicNearClip(orthoNear);
				);
			
				float orthoFar = camera.GetOrthographicFarClip();
				SAND_LEFT_LABEL_TOOLTIP(ImGui::DragFloat("##Far", &orthoFar), "Far", "Objects further from the camera than this value will be clipped",
					camera.SetOrthographicFarClip(orthoFar);
				);
			
				SAND_LEFT_LABEL_TOOLTIP(ImGui::Checkbox("##Fixed Aspect Ratio", &component.FixedAspectRatio), "Fixed Aspect Ratio", "If this is true the cameras aspect ratio will not adjust to the window scale", );
			}
		});

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component)
		{
			SAND_LEFT_LABEL(ImGui::ColorEdit4("##Color", glm::value_ptr(component.Color)), "Color", );
		});
	}

}