#pragma once

#include "ScriptData.h"

namespace Sand
{

	enum class ScriptFieldType
	{
		Unknown,

		Float, Double, Int, UInt,
		Vector2, Vector3, Vector4,

		Color,
	};

	class Scene;

	class ScriptEngine
	{
	public:
		static void Init();
		static void Shutdown();

		static void RegisterModule(uint32_t actorID, const std::string& moduleName);
		static void UnregisterModule(uint32_t actorID);
		static bool IsModuleRegistered(uint32_t actorID);

		static ScriptFieldType MonoTypeToScriptDataType(MonoType* type);

		static MonoObject* Invoke(MonoMethod* method, MonoObject* object, void** params = nullptr, bool errorIfNull = false);

		// Getters
		static MonoImage* GetMonoImage();
		static std::vector<const char*>& GetClientScriptNames();
		static ScriptData& GetScriptDataFromActor(uint32_t actorID);
		static std::unordered_map<uint32_t, ScriptData>& GetScriptDatas();
	private:
		static void LoadClientAssembly();
		static void RegisterInternalCalls();

		friend class Scene;
	};

}