#pragma once

#include <mono/metadata/reflection.h>

namespace Sand
{

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