#pragma once

#include "Sand/Scene/Actor.h"
#include <mono/metadata/assembly.h>

namespace Sand
{

	struct ScriptComponent
	{
		Actor owner{};
	private:
		std::string ModuleName;
	public:
		MonoObject* Object = nullptr;
		MonoClass* Class = nullptr;

		MonoMethod* OnCreateMethod = nullptr;
		MonoMethod* OnUpdateMethod = nullptr;
		MonoMethod* OnDestroyMethod = nullptr;

		void Init(const std::string& nameSpace, const std::string& moduleName, bool awareOfExistence = false);

		void OnCreate();
		void OnUpdate(float ts);
		void OnDestroy();

		ScriptComponent() = default;
		ScriptComponent(const ScriptComponent&) = default;

		void Reset();

		std::string GetModuleName() const 
		{ 
			return ModuleName; 
		}
	};

}