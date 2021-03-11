#pragma once

#include "Sand/Core/Time.h"

namespace Sand
{

	class ScriptRegistry;

	enum class ScriptDataType
	{
		Float = 12, Double = 13,
		Int16 = 6, Int32 = 8, Int64 = 10,
		UInt16 = 7, UInt32 = 9, UInt64 = 11,
		String = 14,
	};

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
		static bool ModuleActive(const std::string& moduleName);
		static void AddModule(const std::string& moduleName);

		static struct ScriptData* GetScriptData(const std::string& moduleName);
	private:
		static void CreateAll();
		static void UpdateAll(Timestep ts);
		static void DestroyAll();
	private:
		static void SetupInternalCalls();

		friend class Scene;
	};

}