#pragma once

#include <mono/metadata/attrdefs.h>
#include <mono/metadata/reflection.h>

namespace Sand
{

	enum class ScriptFieldType;

	enum class FieldFlag
	{
		Public = MONO_FIELD_ATTR_PUBLIC,
		Private = MONO_FIELD_ATTR_PRIVATE,
		Static = MONO_FIELD_ATTR_STATIC,
	};

	struct ScriptField
	{
		ScriptFieldType Type;
		MonoClassField* MonoField = nullptr;

		inline bool HasFlag(FieldFlag flag) const
		{
			SAND_PROFILE_FUNCTION();

			uint32_t fieldFlags = mono_field_get_flags(MonoField);
			return (fieldFlags & static_cast<uint32_t>(flag));
		}
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