#pragma once

#include "Sand/Scene/Actor.h"
#include <mono/metadata/assembly.h>
#include <mono/metadata/reflection.h>

namespace Sand
{

	struct ScriptData
	{
	public:
		ScriptData() = default;

		void OnCreate();
		void OnUpdate(float ts);
		void OnDestroy();
	public:
		MonoObject* Object = nullptr;
		MonoClass* Class = nullptr;

		MonoMethod* OnCreateMethod = nullptr;
		MonoMethod* OnUpdateMethod = nullptr;
		MonoMethod* OnDestroyMethod = nullptr;
	};

}