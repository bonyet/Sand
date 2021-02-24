#include "PropertiesPanel.h"
#include "Sand/Scene/Components.h"
#include "Sand/ImGui/imgui_custom.h"
#include "Sand/Scripting/ScriptComponent.h"
#include "Sand/Scripting/ScriptEngine.h"
#include "Sand/Core/KeyCodes.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#define SAND_LEFT_LABEL(func, label, code) ImGui::TextUnformatted(label); ImGui::NextColumn(); ImGui::SetNextItemWidth(-1); if(func) { code } ImGui::NextColumn();
#define SAND_LEFT_LABEL_TOOLTIP(func, label, tooltip, code) ImGui::TextTooltip(label, tooltip); ImGui::NextColumn(); ImGui::SetNextItemWidth(-1); if(func) { code } ImGui::NextColumn();

namespace Sand
{
	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Actor Actor, UIFunction uiFunction)
	{
		constexpr ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		
		if (!Actor.HasComponent<T>())
			return;

		auto& component = Actor.GetComponent<T>();
		ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

		bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
		ImGui::Tooltip("Right click for more actions", 1.1f);

		if (ImGui::IsItemClicked(1))
			ImGui::OpenPopup("Component_Settings");

		bool removeComponent = false;
		if (ImGui::BeginPopup("Component_Settings"))
		{
			if (ImGui::MenuItem("Reset"))
				component.Reset();
			if (ImGui::MenuItem("Remove"))
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
			Actor.RemoveComponent<T>();

		ImGui::Separator();
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

		if (!tooltip.empty()) {
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

	template<typename T, typename... Args>
	static void DrawComponentMenuItem(const std::string& title, Actor actor, Args&&... args)
	{
		if (ImGui::MenuItem(title.c_str()))
		{
			if (!actor.HasComponent<T>()) {
				actor.AddComponent<T>((args)...);
			}
			else
			{
				SAND_CORE_WARN("{0} component already present on '{1}'", title, actor.GetComponent<TagComponent>().Name);
			}

			ImGui::CloseCurrentPopup();
		}
	}

	void PropertiesPanel::DrawComponentsMenu(Actor actor)
	{
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			DrawComponentMenuItem<CameraComponent>("Camera", actor);
			DrawComponentMenuItem<TransformComponent>("Transform", actor);
			DrawComponentMenuItem<SpriteRendererComponent>("Sprite Renderer", actor);
			DrawComponentMenuItem<Rigidbody2DComponent>("Rigidbody2D", actor);
			DrawComponentMenuItem<BoxCollider2DComponent>("Box Collider 2D", actor, actor.GetComponent<TransformComponent>().Scale);
			DrawComponentMenuItem<ScriptComponent>("Script", actor);

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();
	}

	void PropertiesPanel::DrawComponents(Actor actor)
	{
		if (actor.HasComponent<TagComponent>())
		{
			auto& tag = actor.GetComponent<TagComponent>().Name;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		ImGui::SameLine();
		DrawComponentsMenu(actor);
		ImGui::Separator();

		DrawComponent<TransformComponent>("Transform", actor, [](auto& component)
		{
			DrawVectorControl("Position", component.Position, 0.0f, 100.0f, "The world space position of this actor, in meters");

			// Rotation in euler angles for UI, radians internally so we convert
			glm::vec3 eulerAngles = glm::degrees(component.Rotation);
			DrawVectorControl("Rotation", eulerAngles, 0.0f, 100.0f, "The rotation of this actor in euler angles");
			component.Rotation = glm::radians(eulerAngles);

			DrawVectorControl("Scale", component.Scale, 1.0f, 100.0f, "The scale of this actor in meters");
		});

		DrawComponent<CameraComponent>("Camera", actor, [](auto& component)
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

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", actor, [](auto& component)
		{
			SAND_LEFT_LABEL(ImGui::ColorEdit4("##Color", glm::value_ptr(component.Color)), "Color", );
		});

		DrawComponent<BoxCollider2DComponent>("Box Collider 2D", actor, [](auto& component)
		{
			glm::vec2 bounds = component.GetBounds();
			SAND_LEFT_LABEL(ImGui::DragFloat2("##Bounds", glm::value_ptr(bounds)), "Bounds",
			{
				component.SetBounds(bounds);
			});
		});

		DrawComponent<Rigidbody2DComponent>("Rigidbody2D", actor, [](auto& component)
		{
			{
				const char* rigidbodyTypesStrings[] = { "Static", "Kinematic", "Dynamic" };
				const char* currentRigidbodyTypeString = rigidbodyTypesStrings[(int)component.GetType()];
			
				SAND_LEFT_LABEL(ImGui::BeginCombo("##Type", currentRigidbodyTypeString), "Type",
				{
					for (int i = 0; i < 3; i++)
					{
						bool isSelected = currentRigidbodyTypeString == rigidbodyTypesStrings[i];
						if (ImGui::Selectable(rigidbodyTypesStrings[i], isSelected))
						{
							currentRigidbodyTypeString = rigidbodyTypesStrings[i];
							component.SetType((RigidbodyType)i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				});
			}

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			float friction = component.GetFriction();
			SAND_LEFT_LABEL(ImGui::InputFloat("##Friction", &friction, 0.0f, 0.0f, 2), "Friction",
			{
				component.SetFriction(friction);
			});

			float gravity = component.GetGravityScale();
			SAND_LEFT_LABEL(ImGui::InputFloat("##GravityScale", &gravity, 0.0f, 0.0f, 2), "Gravity Scale",
			{
				component.SetGravityScale(gravity);
			});

			float restitution = component.GetRestitution();
			SAND_LEFT_LABEL(ImGui::InputFloat("##Restitution", &restitution, 0.0f, 0.0f, 2), "Restitution",
			{
				component.SetRestitution(restitution);
			});
		});
	
		DrawComponent<ScriptComponent>("Script", actor, [](auto& component)
		{
			static bool moduleExists = false;

			char buffer[50];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), component.GetModuleName().c_str());

			ImVec4 textColor = moduleExists ? ImVec4{ 0.2f, 0.8f, 0.3f, 1.0f } : ImVec4{ 0.8f, 0.1f, 0.2f, 1.0f };
			ImGui::PushStyleColor(ImGuiCol_Text, textColor);
			
			SAND_LEFT_LABEL(ImGui::InputText("##ModuleName", buffer, sizeof(buffer)), "ModuleName",
			{
				moduleExists = ScriptEngine::ModuleExists(buffer);

				if (!moduleExists)
					component.Reset();

				if (moduleExists && ImGui::IsKeyPressed((int)Keycode::Enter))
					component.Init("Client", buffer);
			});
			
			ImGui::PopStyleColor();

			if (moduleExists)
			{
				component.RenderAllFields();
			}
		});
	}

}