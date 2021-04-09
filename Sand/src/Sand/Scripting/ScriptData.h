#pragma once

#include "Sand/Scene/Actor.h"
#include <mono/metadata/reflection.h>

namespace Sand
{

	enum class ScriptDataType;

	struct ScriptData
	{
	public:
		ScriptData() = default;
	public:
		MonoObject* Object = nullptr;
		MonoClass* Class = nullptr;

		MonoMethod* OnLoadMethod = nullptr;
		MonoMethod* OnCreateMethod = nullptr;
		MonoMethod* OnUpdateMethod = nullptr;
		MonoMethod* OnLateUpdateMethod = nullptr;
		MonoMethod* OnDestroyMethod = nullptr;
	};

}