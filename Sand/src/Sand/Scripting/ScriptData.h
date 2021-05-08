#pragma once

#include <mono/metadata/reflection.h>

namespace Sand
{

	enum class ScriptFieldType;

	struct ScriptField
	{
		ScriptFieldType Type;
		MonoClassField* MonoField = nullptr;
	};

	struct ScriptData
	{
		MonoObject* Object = nullptr;
		MonoClass* Class = nullptr;

		MonoMethod* OnLoadMethod = nullptr;
		MonoMethod* OnCreateMethod = nullptr;
		MonoMethod* OnUpdateMethod = nullptr;
		MonoMethod* OnLateUpdateMethod = nullptr;
		MonoMethod* OnDestroyMethod = nullptr;

		std::vector<ScriptField> Fields;
	};

}