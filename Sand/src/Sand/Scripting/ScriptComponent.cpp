#include "sandpch.h"
#include "ScriptComponent.h"
#include "ScriptEngine.h"
#include <mono/jit/jit.h>

namespace Sand
{

	void ScriptComponent::Init(const std::string& nameSpace, const std::string& moduleName, bool awareOfExistence)
	{
		if (Object)
			return; // Dont init twice

		ModuleName = moduleName;
		SAND_CORE_TRACE("Creating ScriptComponent {0}.{1}", nameSpace, moduleName);

		if (!awareOfExistence && !ScriptEngine::ModuleExists(moduleName))
			return;

		// TODO: profile?
		// idk if reinterpret_cast is necessary
		MonoImage* monoImage = mono_assembly_get_image(reinterpret_cast<MonoAssembly*>(ScriptEngine::GetClientAssembly()));

		MonoClass* klass = mono_class_from_name(monoImage, nameSpace.c_str(), moduleName.c_str());

		Object = mono_object_new(mono_domain_get(), klass);
		OnCreateMethod = mono_class_get_method_from_name(klass, "OnCreate", 0);
		OnUpdateMethod = mono_class_get_method_from_name(klass, "OnUpdate", 1);
		OnDestroyMethod = mono_class_get_method_from_name(klass, "OnDestroy", 0);
	}

	void ScriptComponent::Reset()
	{
		ModuleName = "";
		Object = nullptr;
	}

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