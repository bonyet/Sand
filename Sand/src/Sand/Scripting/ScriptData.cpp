#include "sandpch.h"
#include "ScriptData.h"
#include "ScriptEngine.h"
#include <mono/jit/jit.h>

#include <imgui.h>

namespace Sand
{

	void ScriptData::OnCreate()
	{
		if (!OnCreateMethod)
			return;

		mono_runtime_invoke(OnCreateMethod, Object, NULL, NULL);
	}

	void ScriptData::OnUpdate(float ts)
	{
		void* params[] = { &ts };

		if (!OnUpdateMethod)
			return;

		mono_runtime_invoke(OnUpdateMethod, Object, params, NULL);
	}

	void ScriptData::OnDestroy()
	{
		if (!OnDestroyMethod)
			return;

		mono_runtime_invoke(OnDestroyMethod, Object, NULL, NULL);
	}

}