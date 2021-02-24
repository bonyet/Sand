#pragma once

#include "Sand/Scene/Actor.h"
#include <mono/metadata/assembly.h>
#include <mono/metadata/reflection.h>

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

		void Init(const std::string& nameSpace, const std::string& moduleName, bool awareOfModuleExistence = false);

		void OnCreate();
		void OnUpdate(float ts);
		void OnDestroy();

		ScriptComponent() = default;
		ScriptComponent(const ScriptComponent&) = default;
		~ScriptComponent();

		void Reset();
		void RenderAllFields();
		void RenderField(MonoType* fieldType, MonoClassField* field);

		std::string GetModuleName() const
		{
			return ModuleName; 
		}
	};

}