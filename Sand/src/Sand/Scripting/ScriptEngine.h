#pragma once

#include "Sand/Core/Time.h"

namespace Sand
{

	class ScriptRegistry;

	class ScriptEngine
	{
	public:
		static void Init();
		static void Cleanup();

		static void LoadClientAssembly();

		// void* cause I'm too lazy to sort out include paths
		static void* GetCoreAssembly();
		static void* GetClientAssembly();

		static bool ModuleExists(const std::string& moduleName);
	private:
		static void SetupInternalCalls();
	};

}