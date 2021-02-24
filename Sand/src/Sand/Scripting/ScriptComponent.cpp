#include "sandpch.h"
#include "ScriptComponent.h"
#include "ScriptEngine.h"
#include <mono/jit/jit.h>

#include <imgui.h>

namespace Sand
{

	void ScriptComponent::Init(const std::string& nameSpace, const std::string& moduleName, bool awareOfModuleExistence)
	{
		if (Object)
			return; // Dont init twice

		ModuleName = moduleName;
		SAND_CORE_TRACE("Creating ScriptComponent {0}::{1}", nameSpace, moduleName);

		if (!awareOfModuleExistence && !ScriptEngine::ModuleExists(moduleName))
			return;

		// TODO: profile?
		// idk if reinterpret_cast is necessary
		MonoImage* monoImage = mono_assembly_get_image(reinterpret_cast<MonoAssembly*>(ScriptEngine::GetClientAssembly()));

		Class = mono_class_from_name(monoImage, nameSpace.c_str(), moduleName.c_str());

		Object = mono_object_new(mono_domain_get(), Class);
		OnCreateMethod = mono_class_get_method_from_name(Class, "OnCreate", 0);
		OnUpdateMethod = mono_class_get_method_from_name(Class, "OnUpdate", 1);
		OnDestroyMethod = mono_class_get_method_from_name(Class, "OnDestroy", 0);
	}

	void ScriptComponent::Reset()
	{
		ModuleName = "";
		Object = nullptr;
		Class = nullptr;
	}

	ScriptComponent::~ScriptComponent()
	{
		Reset();
	}

	void ScriptComponent::RenderAllFields()
	{
		if (!Class)
			return; // We haven't initialized yet

		void* iterator = 0;
		MonoClassField* field;
		while (field = mono_class_get_fields(Class, &iterator))
		{
			RenderField(mono_field_get_type(field), field);
		}
	}

#define SAND_LEFT_LABEL(func, label, code) ImGui::TextUnformatted(label); ImGui::NextColumn(); ImGui::SetNextItemWidth(-1); if(func) { code } ImGui::NextColumn();

	void ScriptComponent::RenderField(MonoType* fieldType, MonoClassField* field)
	{
		if (!Object)
			return;

		const char* name = mono_field_get_name(field);

		const std::string fieldIDStr = std::string("##") + std::string(name);
		const char* fieldID = fieldIDStr.c_str();

		ImGui::Columns(2);
		
		const ScriptDataType dataType = (ScriptDataType)mono_type_get_type(fieldType);
		switch (dataType)
		{
			case ScriptDataType::Double:
			case ScriptDataType::Float:
			{
				float value;
				mono_field_get_value(Object, field, &value);
				SAND_LEFT_LABEL(ImGui::InputFloat(fieldID, &value, 0.5f, 1.0f), name,
				{
					mono_field_set_value(Object, field, &value);
				});
				break;
			}
			case ScriptDataType::Int16:
			case ScriptDataType::Int64:
			case ScriptDataType::Int32:
			{
				int value;
				mono_field_get_value(Object, field, &value);
				SAND_LEFT_LABEL(ImGui::InputInt(fieldID, &value, 1), name,
				{
					mono_field_set_value(Object, field, &value);
				});
				break;
			}
			case ScriptDataType::String:
			{
				ImGui::PushStyleColor(ImGuiCol_Text, { 0.8f, 0.2f, 0.3f, 1.0f });
				ImGui::Text("<Unsupported data type>");
				ImGui::PopStyleColor();
				break;
			}
			default:
			{
				ImGui::PushStyleColor(ImGuiCol_Text, { 0.8f, 0.2f, 0.3f, 1.0f });
				ImGui::Text("<Unknown data type>");
				ImGui::PopStyleColor();
			}
		}

		ImGui::Columns(1);
	}
#undef SAND_LEFT_LABEL

	void ScriptComponent::OnCreate()
	{
		if (!OnCreateMethod)
			return;

		mono_runtime_invoke(OnCreateMethod, Object, NULL, NULL);
	}

	void ScriptComponent::OnUpdate(float ts)
	{
		void* params[] = { &ts };

		if (!OnUpdateMethod)
			return;

		mono_runtime_invoke(OnUpdateMethod, Object, params, NULL);
	}

	void ScriptComponent::OnDestroy()
	{
		if (!OnDestroyMethod)
			return;

		mono_runtime_invoke(OnDestroyMethod, Object, NULL, NULL);
	}

}