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
	static void DrawComponentMenuItem(const std::string& title, Actor actor, Args&&... args)
	{
		if (ImGui::MenuItem(title.c_str()))
		{
			if (!actor.HasComponent<T>()) 
			{
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
			DrawComponentMenuItem<ScriptComponent>("Script", actor);

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
		}

		ImGui::SameLine();
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

		DrawComponent<PhysicsComponent>("Physics", actor, [](auto& component)
		{

		});

		DrawComponent<ScriptComponent>("Script", actor, [](auto& component)
		{
			static bool moduleExists = false;

			char buffer[50];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), component.ModuleName.c_str());

			ImVec4 textColor = moduleExists ? ImVec4{ 0.2f, 0.8f, 0.3f, 1.0f } : ImVec4{ 0.8f, 0.1f, 0.2f, 1.0f };
			ImGui::PushStyleColor(ImGuiCol_Text, textColor);
			
			SAND_LEFT_LABEL(ImGui::InputText("##ModuleName", buffer, sizeof(buffer)), "ModuleName",
			{
				component.ModuleName = buffer;
				moduleExists = ScriptEngine::ModuleExists(component.ModuleName);
			});

			if (moduleExists && ImGui::IsKeyPressed((int)Keycode::Enter, false) && !ScriptEngine::ModuleActive(component.ModuleName)) 
			{
				ScriptEngine::AddModule(component.ModuleName);
			}
			
			ImGui::PopStyleColor();

			if (moduleExists)
			{
				const ScriptData* sdata = ScriptEngine::GetScriptData(component.ModuleName);
				if (sdata)
					RenderAllScriptFields(sdata);
			}
		});
	}

	static void RenderScriptField(const ScriptData* const scriptData, MonoType* type, MonoClassField* field);

	static void RenderAllScriptFields(const ScriptData* const scriptData)
	{
		void* iterator = 0;
		MonoClassField* field;
		while (field = mono_class_get_fields(scriptData->Class, &iterator))
		{
			RenderScriptField(scriptData, mono_field_get_type(field), field);
		}
	}

	static void RenderScriptField(const ScriptData* const scriptData, MonoType* type, MonoClassField* field)
	{
		const char* name = mono_field_get_name(field);

		const std::string fieldIDStr = std::string("##") + std::string(name);
		const char* fieldID = fieldIDStr.c_str();

		ImGui::Columns(2);

		const ScriptDataType dataType = (ScriptDataType)mono_type_get_type(type);
		switch (dataType)
		{
		case ScriptDataType::Double:
		case ScriptDataType::Float:
		{
			float value;
			mono_field_get_value(scriptData->Object, field, &value);
			SAND_LEFT_LABEL(ImGui::InputFloat(fieldID, &value, 0.5f, 1.0f), name,
			{
				mono_field_set_value(scriptData->Object, field, &value);
			});
			break;
		}
		case ScriptDataType::UInt16:
		case ScriptDataType::UInt64:
		case ScriptDataType::UInt32:
		case ScriptDataType::Int16:
		case ScriptDataType::Int64:
		case ScriptDataType::Int32:
		{
			int value;
			mono_field_get_value(scriptData->Object, field, &value);
			SAND_LEFT_LABEL(ImGui::InputInt(fieldID, &value, 1), name,
			{
				mono_field_set_value(scriptData->Object, field, &value);
			});
			break;
		}
		case ScriptDataType::String:
		default:
		{
			ImGui::PushStyleColor(ImGuiCol_Text, { 0.8f, 0.2f, 0.3f, 1.0f });
			ImGui::Text("<Unknown data type>");
			ImGui::PopStyleColor();
		}
		}

		ImGui::Columns(1);
	}

}