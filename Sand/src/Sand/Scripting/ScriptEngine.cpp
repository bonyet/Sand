#include "sandpch.h"
#include "ScriptEngine.h"

#include "Sand/Core/Input.h"

#include "Sand/Debug/Debug.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <vector>
#include <string>
#include <filesystem>

namespace Sand
{
	static MonoDomain* s_MonoDomain = nullptr;
	static MonoImage* s_MonoImage = nullptr;
	static MonoAssembly* s_MonoAssembly = nullptr;

	static MonoAssembly* s_ClientMonoAssembly = nullptr;
	static MonoImage* s_ClientMonoImage = nullptr;

	void ScriptEngine::Init()
	{
		mono_set_dirs("C:\\Program Files\\Mono\\lib", "C:\\Program Files\\Mono\\etc");
		mono_set_assemblies_path("lib");
		
		MonoDomain* domain = mono_jit_init("Sand");

		s_MonoDomain = mono_domain_create_appdomain((char*)"Sand-Runtime", nullptr);

		// Core
		const char* path = "Sand-CSCore.dll";
		s_MonoAssembly = mono_domain_assembly_open(s_MonoDomain, path);
		SAND_CORE_ASSERT(s_MonoAssembly, "Failed to load core MonoAssembly");
		mono_assembly_set_main(s_MonoAssembly);

		s_MonoImage = mono_assembly_get_image(s_MonoAssembly);

		LoadClientAssembly();

		SetupInternalCalls();
	}
	
	void ScriptEngine::Cleanup()
	{
		mono_jit_cleanup(s_MonoDomain);
	}

	void ScriptEngine::LoadClientAssembly()
	{
		MonoDomain* domain = nullptr;
		if (s_MonoDomain)
		{
			domain = mono_domain_create_appdomain("Sand Client", nullptr);
			mono_domain_set(domain, true);
		}

		// Client
		// TODO: get correct client dll path (set on project create)
		const char* path = "D:\\dev\\Sand\\bin\\Debug-windows-x86_64\\Sand-CSClient\\Sand-CSClient.dll";
		s_ClientMonoAssembly = mono_domain_assembly_open(s_MonoDomain, path);
		SAND_CORE_ASSERT(s_ClientMonoAssembly, "Failed to load client MonoAssembly");
		s_ClientMonoImage = mono_assembly_get_image(s_ClientMonoAssembly);
	}

	void* ScriptEngine::GetCoreAssembly()
	{
		return s_MonoAssembly;
	}

	void* ScriptEngine::GetClientAssembly()
	{
		return s_ClientMonoAssembly;
	}

	bool ScriptEngine::ModuleExists(const std::string& moduleName)
	{
		MonoClass* klass = mono_class_from_name(s_ClientMonoImage, "Client", moduleName.c_str());
		return klass != nullptr;
	}

#pragma region InternalFunctions
	// ===== LOGGING =====
	static void PrintInfo_Native(MonoString* string)
	{
		char* str = mono_string_to_utf8(string);
		if (str)
			SAND_CORE_INFO(str);
		mono_free(str);
	}
	static void PrintWarn_Native(MonoString* string)
	{
		char* str = mono_string_to_utf8(string);
		if (str)
			SAND_CORE_WARN(str);
		mono_free(str);
	}
	static void PrintError_Native(MonoString* string)
	{
		char* str = mono_string_to_utf8(string);
		if (str)
			SAND_CORE_ERROR(str);
		mono_free(str);
	}
	//====== INPUT ========
	static inline bool Input_IsKeyPressed(Keycode button)
	{
		return Input::IsKeyPressed(button);
	}
	static inline bool Input_WasKeyPressed(Keycode button)
	{
		return Input::WasKeyPressed(button);
	}
	static inline bool Input_IsMouseButtonPressed(Mousecode button)
	{
		return Input::IsKeyPressed(button);
	}
	static inline bool Input_WasMouseButtonPressed(Mousecode button)
	{
		return Input::WasKeyPressed(button);
	}
	// ===== COMPONENTS =====

	static void TransformComponent_SetPosition(MonoObject* object)
	{

	}
	static void TransformComponent_SetRotation(MonoObject* object)
	{

	}
	static void TransformComponent_SetScale(MonoObject* object)
	{

	}
#pragma endregion

	void ScriptEngine::SetupInternalCalls()
	{
		mono_add_internal_call("Sand.Log::Info", &PrintInfo_Native);
		mono_add_internal_call("Sand.Log::Warn", &PrintWarn_Native);
		mono_add_internal_call("Sand.Log::Error", &PrintError_Native);

		mono_add_internal_call("Sand.Input::IsKeyPressed_Native", &Input_IsKeyPressed);
		mono_add_internal_call("Sand.Input::WasKeyPressed_Native", &Input_WasKeyPressed);

		mono_add_internal_call("Sand.Input::IsMousePressed_Native", &Input_IsMouseButtonPressed);
		mono_add_internal_call("Sand.Input::WasMousePressed_Native", &Input_WasMouseButtonPressed);

		mono_add_internal_call("Sand.TransformComponent::SetPosition_Native", &TransformComponent_SetPosition);
		mono_add_internal_call("Sand.TransformComponent::SetRotation_Native", &TransformComponent_SetRotation);
		mono_add_internal_call("Sand.TransformComponent::SetScale_Native", &TransformComponent_SetScale);
	}
	
}
