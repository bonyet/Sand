#pragma once

#include "Sand/Scene/Actor.h"
#include <mono/metadata/reflection.h>

namespace Sand
{

	enum class ScriptDataType;

	struct ScriptField
	{
		ScriptField(MonoClassField* monoField, const char* name, bool ispublic, ScriptDataType type)
			: mMonoField(monoField), mName(name), mPublic(ispublic), mType(type)
		{
		}

		const bool IsPublic() const { return mPublic; }
		const char* GetName() const { return mName; }
		const ScriptDataType GetType() const { return mType; }
		MonoClassField* GetField() const { return mMonoField; }
	private:
		bool mPublic;
		ScriptDataType mType;
		const char* mName;

		MonoClassField* mMonoField = nullptr;
	};

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

		std::vector<ScriptField> Fields;
	};

}