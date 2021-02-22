#include "sandpch.h"
#include "ScriptEngine.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

#include "Sand/Debug/Debug.h"

#include <vector>
#include <string>

namespace Sand
{
	static MonoDomain* s_MonoDomain = nullptr;
	static MonoAssembly* s_MonoAssembly = nullptr;
	static MonoImage* s_MonoImage = nullptr;

	std::vector<std::tuple<std::string, glm::vec3, std::string>> Debug::m_DebugMessages;

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

	void ScriptEngine::Init()
	{
		mono_set_dirs("C:\\Program Files\\Mono\\lib", "C:\\Program Files\\Mono\\etc");

		const char* domain_name = "Sand_ClientCS_Domain";
		s_MonoDomain = mono_jit_init(domain_name);

		s_MonoAssembly = mono_domain_assembly_open(s_MonoDomain, "D:\\dev\\Sand\\bin\\Debug-windows-x86_64\\Sand-CSClient\\Sand-CSClient.dll");
		SAND_CORE_ASSERT(s_MonoAssembly, "Failed to load MonoAssembly.");

		s_MonoImage = mono_assembly_get_image(s_MonoAssembly);

		mono_add_internal_call("Sand.Log::Info", &PrintInfo_Native);
		mono_add_internal_call("Sand.Log::Warn", &PrintWarn_Native);
		mono_add_internal_call("Sand.Log::Error", &PrintError_Native);

		// Try to get the main class to do stuff (WIP)
		{
			auto klass = mono_class_from_name(s_MonoImage, "Client", "App");
			MonoObject* classInstance = mono_object_new(s_MonoDomain, klass);
			MonoMethod* runMethod = mono_class_get_method_from_name(klass, "Run", 0);
			mono_runtime_invoke(runMethod, classInstance, nullptr, NULL);
		}

		//int argc = 1;
		//char* argv[1] = { (char*)"CS" };
		//int retval = mono_jit_exec(s_MonoDomain, s_MonoAssembly, argc, argv);
	}
	
	void ScriptEngine::Cleanup()
	{
		// Free mem
		mono_jit_cleanup(s_MonoDomain);

		// nullify all that good stuff
		s_MonoDomain = nullptr;
		s_MonoAssembly = nullptr;
	}
	
}
