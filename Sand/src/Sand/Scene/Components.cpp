#include "sandpch.h"
#include "Components.h"

#include "Sand/Scripting/ScriptEngine.h"

namespace Sand
{

	void ScriptComponent::Activate()
	{
		if (!ScriptEngine::ModuleExists(ModuleName))
			return;

		ScriptEngine::AddModule(owner, ModuleName);
	}

	void ScriptComponent::Deactivate()
	{
		ScriptEngine::DeactivateModule(owner);
	}

}