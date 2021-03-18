#include "PropertiesPanel.h"
#include "Sand/Scene/Components.h"
#include "Sand/ImGui/imgui_custom.h"

#include "Sand/Scripting/ScriptData.h"
#include "Sand/Scripting/ScriptEngine.h"
#include "Sand/Core/KeyCodes.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include "Sand/Utils/PlatformUtils.h"

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

		std::string nodeName = name;
		if constexpr (std::is_base_of_v<T, ScriptComponent>)
			nodeName = component.ModuleName + " :: (Script)";
		
		bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, nodeName.c_str());

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
			SAND_LEFT_LABEL_TOOLTIP(ImGui::DragFloat3("##Drag", glm::value_ptr(vector), 0.05f, 0.0f, 0.0f, "%.2f"), label.c_str(), tooltip.c_str(), );
		}
		else {
			SAND_LEFT_LABEL(ImGui::DragFloat3("##Drag", glm::value_ptr(vector), 0.05f, 0.0f, 0.0f, "%.2f"), label.c_str(), );
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
	static bool DrawComponentMenuItem(const std::string& title, Actor actor, Args&&... args)
	{
		bool result = false;
		if (ImGui::MenuItem(title.c_str()))
		{
			if (!actor.HasComponent<T>()) 
			{
				actor.AddComponent<T>((args)...);
				result = true;
			}
			else
			{
				SAND_CORE_WARN("{0} component already present on '{1}'", title, actor.GetComponent<TagComponent>().Name);
			}

			ImGui::CloseCurrentPopup();
		}

		return result;
	}

	void PropertiesPanel::DrawComponentsMenu(Actor actor)
	{
		SAND_PROFILE_FUNCTION();

		ImGui::PushItemWidth(-1);

		ImGui::PushStyleColor(ImGuiCol_Button,        { 0.20f, 0.40f, 0.30f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.10f, 0.30f, 0.20f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive,  { 0.15f, 0.35f, 0.25f, 1.0f });
		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");
		ImGui::PopStyleColor(3);

		if (ImGui::BeginPopup("AddComponent", false))
		{
			// Misc / Base
			if (ImGui::BeginMenu("Basic"))
			{
				DrawComponentMenuItem<TransformComponent>("Transform", actor);

				ImGui::EndMenu();
			}

			// Graphics
			if (ImGui::BeginMenu("Graphics"))
			{
				DrawComponentMenuItem<CameraComponent>("Camera", actor);
				DrawComponentMenuItem<TextureComponent>("Texture", actor);
				DrawComponentMenuItem<SpriteRendererComponent>("Sprite Renderer", actor);
				
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Physics"))
			{
				DrawComponentMenuItem<PhysicsComponent>("Physics", actor);

				ImGui::EndMenu();
			}

			ImGui::Separator();

			// Scripting
			if (ImGui::BeginMenu("Scripts"))
			{
				for (auto pKlass : ScriptEngine::GetCachedClientScripts())
				{
					MonoClass* klass = (MonoClass*)pKlass;
					const char* name = mono_class_get_name(klass);

					if (DrawComponentMenuItem<ScriptComponent>(name, actor))
					{
						auto& addedScript = actor.GetComponent<ScriptComponent>();
						addedScript.ModuleName = name;
						addedScript.Activate();
					}
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Misc"))
			{
				DrawComponentMenuItem<AnimatorComponent>("Animator", actor);

				ImGui::EndMenu();
			}

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();
	}

	inline static void SetColumnsMinSpacing(float spacing) {
		static ImGuiStyle& style = ImGui::GetStyle();
		style.ColumnsMinSpacing = spacing;
	}

	void PropertiesPanel::DrawComponents(Actor actor)
	{
		SAND_PROFILE_FUNCTION();

		float itemWidth = 0.0f;
		if (actor.HasComponent<TagComponent>())
		{
			auto& tag = actor.GetComponent<TagComponent>().Name;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth() - 150);
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}

			itemWidth = ImGui::CalcItemWidth();
			ImGui::SameLine(ImGui::GetContentRegionAvailWidth() - 180);
			ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f, 1.0f, 1.0f, 0.7f });

			ImGui::Text(("= " + std::to_string((uint32_t)actor)).c_str());
			
			ImGui::PopStyleColor();
		}

		ImGui::SameLine(ImGui::GetContentRegionAvailWidth() - 140);
		DrawComponentsMenu(actor);
		ImGui::Separator();

		float defaultColumnSpacing = ImGui::GetStyle().ColumnsMinSpacing;

		SetColumnsMinSpacing(85.0f);
		DrawComponent<TransformComponent>("Transform", actor, [](auto& component)
		{
			ImGuiIO& io = ImGui::GetIO();
			auto boldFont = io.Fonts->Fonts[1];

			// Position
			SAND_LEFT_LABEL_TOOLTIP(ImGui::DragFloat2("##Position", glm::value_ptr(component.Position), 0.05f, 0.0f, 0.0f, "%.2f"), "Position", "The world space position of this actor in meters", );

			SAND_LEFT_LABEL_TOOLTIP(ImGui::DragFloat2("##Scale", glm::value_ptr(component.Scale), 0.05f, 0.05f, 0.0f, "%.2f"), "Scale", "The scale of this actor in meters", );

			// Rotation
			float eulerAngles = glm::degrees(component.Rotation);
			SAND_LEFT_LABEL_TOOLTIP(ImGui::DragFloat("##Rotation", &eulerAngles, 0.1f, 0.0f, 0.0f, "%.2f"), "Rotation", "The rotation of this actor in euler angles", );
			component.Rotation = glm::radians(eulerAngles);
		});
		SetColumnsMinSpacing(defaultColumnSpacing);
		
		SetColumnsMinSpacing(110.0f);
		DrawComponent<CameraComponent>("Camera", actor, [](auto& component)
		{
			auto& camera = component.Camera;

			SAND_LEFT_LABEL_TOOLTIP(ImGui::Checkbox("##Primary", &component.Primary), "Primary", "Primary cameras render the scene", );

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
		});
		SetColumnsMinSpacing(defaultColumnSpacing);

		SetColumnsMinSpacing(100.0f);
		DrawComponent<TextureComponent>("Texture", actor, [](auto& component)
		{
			ImGui::Columns(1);
			if (component.IsTextured())
			{								
				auto textureID = component.Texture->GetID();

				if (ImGui::ImageButton((ImTextureID)textureID, { 100, 100 }, { 0, 1 }, { 1, 0 }))
				{
					std::string chosenFilepath = FileDialogs::OpenFile("Image (*.jpg);(*.png)\0*.png;*.jpg;\0");

					if (!chosenFilepath.empty() && chosenFilepath != component.Texture->GetPath())
						component.Texture = Texture2D::Create(chosenFilepath);
				}
				if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.75f)
				{
					ImGui::BeginTooltip();
					auto textureFilepath = component.Texture->GetPath();
					ImGui::Text(textureFilepath.c_str());
					ImGui::EndTooltip();
				}

				ImGui::Columns(2);
				SAND_LEFT_LABEL(ImGui::DragFloat("##TilingFactor", &component.TilingFactor, 0.1f, 0.05f, 50.0f, "%.2f", 1.0f), "Tiling Factor", );
			}
			else
			{
				ImGui::TextColored({ 0.9f, 0.3f, 0.2f, 1.0f }, "No Texture Active");
				ImGui::SameLine();
				if (ImGui::Button("Load one, then!", { 130, 25 }))
				{
					std::string chosenFilepath = FileDialogs::OpenFile("Image (*.jpg);(*.png)\0*.png;*.jpg;\0");
					if (!chosenFilepath.empty())
						component.Texture = Texture2D::Create(chosenFilepath);
				}
			}
		});
		SetColumnsMinSpacing(defaultColumnSpacing);

		SetColumnsMinSpacing(80.0f);
		DrawComponent<SpriteRendererComponent>("Sprite Renderer", actor, [](auto& component)
		{
			SAND_LEFT_LABEL(ImGui::ColorEdit4("##Color", glm::value_ptr(component.Color)), "Color", );
		});
		SetColumnsMinSpacing(defaultColumnSpacing);

		DrawComponent<ScriptComponent>("Script", actor, [=](auto& component)
		{
			const ScriptData* sdata = ScriptEngine::GetScriptData(actor, component.ModuleName);
			RenderAllScriptFields(sdata);
		});
	
		DrawComponent<PhysicsComponent>("Physics", actor, [](auto& component)
		{
			auto& body = component.Body;

			{
				const char* items[] = { "Static", "Kinematic", "Dynamic" };
				const char* currentItem = items[(int)body.GetType()];

				SAND_LEFT_LABEL(ImGui::BeginCombo("##Type", currentItem), "Type",
				{
					for (int n = 0; n < IM_ARRAYSIZE(items); n++)
					{
						bool isSelected = (currentItem == items[n]);
						if (ImGui::Selectable(items[n], isSelected))
						{
							currentItem = items[n];
							if (isSelected)
								ImGui::SetItemDefaultFocus();

							body.SetType((PhysicsBodyType)n);
						}
					}
					ImGui::EndCombo();
				});
			}

			ImGui::Separator();
			float gravity = body.GetGravityScale();
			SAND_LEFT_LABEL(ImGui::InputFloat("##GravityScale", &gravity), "Gravity Scale",
				body.SetGravityScale(gravity);
			);
			float friction = body.GetFriction();
			SAND_LEFT_LABEL(ImGui::InputFloat("##Friction", &friction), "Friction",
				body.SetFriction(friction);
			);
			float mass = body.GetMass();
			SAND_LEFT_LABEL(ImGui::InputFloat("##Mass", &mass), "Mass",
				body.SetMass(mass);
			);
			ImGui::Separator();

			bool fixedRotation = body.GetFixedRotation();
			SAND_LEFT_LABEL(ImGui::Checkbox("##FixedRotation", &fixedRotation), "Fixed Rotation",
				body.SetFixedRotation(fixedRotation);
			);

		});

		DrawComponent<AnimatorComponent>("Animator", actor, [](auto& component)
		{
			auto& animator = component.Animator;
			uint32_t fps = animator.GetAnimation().GetFPS();
			
			{
				constexpr uint32_t framerates[] = { 10, 16, 24, 30, 60 };
				
				const char* items[] = { "10", "16", "24", "30", "60" };
				static const char* currentItem = NULL;

				SAND_LEFT_LABEL(ImGui::BeginCombo("##FPS", currentItem), "Frames Per Second",
				{
					for (int n = 0; n < IM_ARRAYSIZE(items); n++)
					{
						bool isSelected = (currentItem == items[n]);
						if (ImGui::Selectable(items[n], isSelected))
						{
							currentItem = items[n];
							if (isSelected)
								ImGui::SetItemDefaultFocus();

							animator.GetAnimation().SetFPS(framerates[n]);
						}
					}
					ImGui::EndCombo();
				});
			}

			ImGui::Separator();
			SAND_LEFT_LABEL_TOOLTIP(ImGui::Checkbox("##Loop", &animator.Loop), "Loop", "Whether or not this animation will loop", );
		});
	}

	static void RenderScriptField(const ScriptField& field, const ScriptData* const scriptData);

	static void RenderAllScriptFields(const ScriptData* const scriptData)
	{
		for (auto field : scriptData->Fields)
		{
			if (!field.IsPublic())
				continue; // dont show private fields in inspector

			RenderScriptField(field, scriptData);
		}
	}

	static void RenderScriptField(const ScriptField& field, const ScriptData* const scriptData)
	{
		SAND_PROFILE_FUNCTION();

		const std::string fieldIDStr = std::string("##") + std::string(field.GetName());
		const char* fieldID = fieldIDStr.c_str();

		ImGui::Columns(2);

		switch (field.GetType())
		{
			case ScriptDataType::Double:
			case ScriptDataType::Float:
			{
				float value;
				mono_field_get_value(scriptData->Object, field.GetField(), &value);
				SAND_LEFT_LABEL(ImGui::InputFloat(fieldID, &value, 0.5f, 1.0f), field.GetName(),
				{
					mono_field_set_value(scriptData->Object, field.GetField(), &value);
				});
				break;
			}
			case ScriptDataType::UInt:
			case ScriptDataType::Int:
			{
				int value;
				mono_field_get_value(scriptData->Object, field.GetField(), &value);
				SAND_LEFT_LABEL(ImGui::InputInt(fieldID, &value, 1), field.GetName(),
				{
					mono_field_set_value(scriptData->Object, field.GetField(), &value);
				});
				break;
			}
			case ScriptDataType::Vector2:
			{
				glm::vec2 value;
				mono_field_get_value(scriptData->Object, field.GetField(), &value);
				SAND_LEFT_LABEL(ImGui::DragFloat2(fieldID, glm::value_ptr(value), 0.5f), field.GetName(),
				{
					mono_field_set_value(scriptData->Object, field.GetField(), &value);
				});
				break;
			}
			case ScriptDataType::Vector3:
			{
				glm::vec3 value;
				mono_field_get_value(scriptData->Object, field.GetField(), &value);
				SAND_LEFT_LABEL(ImGui::DragFloat3(fieldID, glm::value_ptr(value), 0.5f), field.GetName(),
				{
					mono_field_set_value(scriptData->Object, field.GetField(), &value);
				});
				break;
			}
			case ScriptDataType::Vector4:
			{
				glm::vec4 value;
				mono_field_get_value(scriptData->Object, field.GetField(), &value);
				SAND_LEFT_LABEL(ImGui::DragFloat4(fieldID, glm::value_ptr(value), 0.5f), field.GetName(),
				{
					mono_field_set_value(scriptData->Object, field.GetField(), &value);
				});
				break;
			}
			case ScriptDataType::Color:
			{
				glm::vec4 value;
				mono_field_get_value(scriptData->Object, field.GetField(), &value);
				SAND_LEFT_LABEL(ImGui::ColorEdit4(fieldID, glm::value_ptr(value)), field.GetName(),
				{
					mono_field_set_value(scriptData->Object, field.GetField(), &value);
				});
				break;
			}
			case ScriptDataType::Unknown:
			{
				ImGui::Text("Unknown data type.");
			}
		}

		ImGui::Columns(1);
	}

}