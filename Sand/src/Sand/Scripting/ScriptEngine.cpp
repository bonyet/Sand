#include "sandpch.h"
#include "ScriptEngine.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

#include "Sand/Debug/Debug.h"

#include <vector>
#include <string>

#include <winioctl.h>

namespace Sand
{
	static MonoDomain* s_MonoDomain = nullptr;
	static MonoImage* s_MonoImage = nullptr;
	static MonoAssembly* s_MonoAssembly = nullptr;

	static MonoAssembly* s_ClientMonoAssembly = nullptr;
	static MonoImage* s_ClientMonoImage = nullptr;

#pragma region InternalFunctions
	static void PrintInfo_Native(MonoString* string)
	{
		char* str = mono_string_to_utf8(string);
		SAND_CORE_INFO(str);
		mono_free(str);
	}
	static void PrintWarn_Native(MonoString* string)
	{
		char* str = mono_string_to_utf8(string);
		SAND_CORE_WARN(str);
		mono_free(str);
	}
	static void PrintError_Native(MonoString* string)
	{
		char* str = mono_string_to_utf8(string);
		SAND_CORE_ERROR(str);
		mono_free(str);
	}
#pragma endregion

	void ScriptEngine::Init()
	{
		mono_set_dirs("C:\\Program Files\\Mono\\lib", "C:\\Program Files\\Mono\\etc");
		mono_set_assemblies_path("../Sand/vendor/Mono/lib");
		
		MonoDomain* domain = mono_jit_init("Sand");

		s_MonoDomain = mono_domain_create_appdomain((char*)"Sand-Runtime", nullptr);

		// Core
		const char* path = "D:\\dev\\Sand\\bin\\Debug-windows-x86_64\\Sand-CSCore\\Sand-CSCore.dll";
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

	void ScriptEngine::SetupInternalCalls()
	{
		mono_add_internal_call("Sand.Log::Info", &PrintInfo_Native);
		mono_add_internal_call("Sand.Log::Warn", &PrintWarn_Native);
		mono_add_internal_call("Sand.Log::Error", &PrintError_Native);
	}
	
}
